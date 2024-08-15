#include <algorithm>
#include <cstring>

#include <filesystem>
#include <boost/algorithm/string.hpp>

#include <img/img.h>
namespace criterion::img {
    ImgStream::ImgStream(std::list<FileStream>&& files) :
        images(std::move(files)) {
        for (auto& img : images) {
            openImg(img);
        }
    }

    ImgStream::ImgStream(const std::vector<std::filesystem::path>& directories) {
        for (const auto& directory : directories) {
            collectImg(directory);
        }
    }

    constexpr u32 alignedImgData(const u32 size) {
        return size + (2048 - 1) & ~(2048 - 1);
    }

    void ImgStream::openImg(FileStream& io) {
        std::fstream imageFile(io.path, std::ios::in | std::ios::binary);
        io.swap(imageFile);

        io.seekg(0);
        std::vector<char> buffer(sizeof(img));
        io.read(&buffer[0], static_cast<std::streamsize>(buffer.size()));
        if (!io.gcount())
            throw std::runtime_error("Error reading from file");

        std::memcpy(&img, &buffer[0], sizeof(img));
        bool checked{};
        for (const auto& accepted : {"VER2"}) {
            if (accepted == std::string_view(&img.version[0], 4))
                checked = true;
        }
        if (!checked) {
            throw std::runtime_error("IMG file not accepted, unrecognized version");
        }

        for (u32 count{};
            count < img.itemCount; count++) {
            entries.emplace_back(io);
            }

        u32 offset{};
        for (u32 index{}; index < entries.size(); index++) {
            auto iterator{std::begin(entries)};
            std::advance(iterator, index);

            populateDatabase(*iterator, io);
            status.requiredSize += entrySize;
            offset += iterator->info.streamingSize * sectorSize;
        }
        status.offset += offset / sectorSize;
        status.requiredSize += offset;
        status.requiredSize += sizeof(img);

        status.requiredSize = alignedImgData(status.requiredSize);

        const auto position{io.tellg()};
        io.seekg(0, std::ios::end);
        const auto fileSize{io.tellg()};

        io.seekg(position, std::ios::beg);
        if (status.requiredSize != fileSize) {
            throw std::runtime_error("IMG file too small");
        }
    }

    void ImgStream::collectImg(const std::filesystem::path& directory) {
        img.version = {'V', 'E', 'R', '2'};

        std::filesystem::directory_iterator walker(directory);
        status.offset *= sectorSize;
        for ([[maybe_unused]] const auto& entry : walker) {
            status.offset += entrySize;
        }
        status.offset += sizeof(img);
        status.offset = alignedImgData(status.offset);
        status.requiredSize += status.offset;
        status.offset /= sectorSize;

        walker = std::filesystem::directory_iterator(directory);

        for (const auto& entry : walker) {
            entries.emplace_back(status, entry);
            images.emplace_back(entry.path());
        }
        img.itemCount = entries.size();
        status.requiredSize = alignedImgData(status.requiredSize);

        for (u32 index{}; index < entries.size(); index++) {
            auto entryIt{std::begin(entries)};
            auto imageIt{std::begin(images)};
            std::advance(entryIt, index);
            std::advance(imageIt, index);
            populateDatabase(*entryIt, *imageIt);
        }
    }

    void ImgStream::populateDatabase(DirectoryEntry& record, FileStream& io) {
        auto sanitizedKey{record.name};
        const auto keyBegin{std::begin(sanitizedKey)};
        const auto keyEnd{std::end(sanitizedKey)};
        std::transform(keyBegin, keyEnd, keyBegin, ::tolower);

        if (!database.contains(sanitizedKey))
            database.emplace(sanitizedKey, DataEntry(record, io));

        if (sanitizedKey.ends_with(".ipl")) {
            iplFiles.push_back(sanitizedKey);
        }
    }

    void ImgStream::excludeFiles(const std::string& files) {
        std::vector<std::string> container;
        split(container, files, boost::is_any_of(",;+"));

        for (const auto& remove : container) {
            // ReSharper disable once CppUseRangeAlgorithm
            const auto erase{std::find_if(std::begin(entries), std::end(entries), [&](const auto& directory) -> bool {
                return directory.name == remove;
            })};
            if (erase == std::end(entries))
                continue;

            database.erase(remove);
            status.requiredSize -= erase->info.streamingSize * sectorSize;
            status.requiredSize -= entrySize;
            entries.erase(erase);
        }

        EntryStatus update{1, 0};
        for (auto& entry : entries) {
            entry.info.offset = update.offset;
            update.offset += entry.info.streamingSize;
        }
        status.requiredSize = alignedImgData(status.requiredSize);
    }

    void ImgStream::addFiles(const std::vector<std::filesystem::path>& files) { // NOLINT(*-no-recursion)
        for (const auto& append : files) {
            if (!exists(append)) {
                throw std::runtime_error("File does not exist");
            }
            if (is_directory(append)) {
                std::vector<std::filesystem::path> direct{};
                // ReSharper disable once CppTooWideScopeInitStatement
                const std::filesystem::directory_iterator walker(append);
                for (const auto& inDir : walker) {
                    direct.emplace_back(inDir.path());
                }
                addFiles(direct);
                continue;
            }
            if (database.contains(append.filename()))
                continue;

            entries.emplace_back(status, append);
            images.emplace_back(append, std::ios::binary | std::ios::in);

            img.itemCount++;
            populateDatabase(entries.back(), images.back());
        }
    }
}
