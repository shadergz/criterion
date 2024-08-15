#pragma once
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <list>

#include <img/data_entry.h>
namespace criterion::img {

    class ImgStream {
    public:
        explicit ImgStream(std::list<FileStream>&& files);
        explicit ImgStream(const std::vector<std::filesystem::path>& directories);

        void extract(const std::filesystem::path& output);
        [[nodiscard]] std::vector<TaggedStream> extract();

        void compress(const std::filesystem::path& output);
        void excludeFiles(const std::string& files);
        void addFiles(const std::vector<std::filesystem::path>& files);

    private:
        void openImg(FileStream& io);
        void collectImg(const std::filesystem::path& directory);
        void populateDatabase(DirectoryEntry& record, FileStream& io);

        ImgHeader img{};
        std::list<DirectoryEntry> entries{};
        std::vector<std::string> iplFiles{};
        std::list<FileStream> images{};
        // Store temporary information to properly reconstruct the file structure in memory
        EntryStatus status{};

        std::unordered_map<std::string, DataEntry> database{};
    };
}
