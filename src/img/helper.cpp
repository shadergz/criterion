#include <cstring>
#include <print>

#include <img/img.h>
namespace criterion::img {

    void ImgStream::extract(const std::filesystem::path& output) {
        if (output.empty())
            return;

        for (const auto& [entry, record] : database ) {
            std::print("Decompressing the file: {}\n", entry);

            const auto entryFilename{output / std::filesystem::path{entry}};
            std::fstream entryIo;
            entryIo.open(entryFilename, std::ios::binary | std::ios::out);

            auto data{record.getData()};
            entryIo.write(reinterpret_cast<char*>(&data[0]), static_cast<std::streamsize>(data.size()));
            entryIo.close();
        }
    }

    std::vector<TaggedStream> ImgStream::extract() {
        std::vector<TaggedStream> streams;

        for (const auto& [entry, record] : database) {
            std::print("Decompressing the file: {}\n", entry);
            TaggedStream stream{entry};

            auto data{record.getData()};
            stream.write(reinterpret_cast<char*>(&data[0]), static_cast<std::streamsize>(data.size()));
            // streams.push_back(std::move(stream));
        }
        return streams;
    }

    void ImgStream::compress(const std::filesystem::path& output) {
        if (output.empty())
            return;

        FileStream io{output, std::ios::binary | std::ios::out};
        if (file_size(output) != status.requiredSize)
            resize_file(output, status.requiredSize);

        io.write(reinterpret_cast<char*>(&img), sizeof(img));
        std::array<char, 24> entryName{};

        entries.sort();

        for (const auto& [record, entry] : entries) {
            // ReSharper disable once CppLocalVariableMayBeConst
            std::add_pointer_t<char> ptr{};
            auto pointer{std::addressof(record)};
            std::memcpy(&ptr, &pointer, sizeof(ptr));
            io.write(ptr, sizeof(record));

            std::ranges::fill(entryName, 0);
            std::memcpy(&entryName[0], &entry[0], entry.size());
            io.write(&entryName[0], entryName.size());
        }
        std::vector<u8> content(4096);
        for (const auto& [entry, record] : database) {
            std::print("Compressing the file: {}\n", entry);
            record.getData(io, content);
        }
        io.close();
    }
}