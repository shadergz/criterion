#pragma once
#include <array>
#include <filesystem>

#include <types.h>
namespace criterion::img {
    constexpr auto sectorSize{2048};

    struct EntryStatus {
        u32 offset;
        u32 requiredSize;
    };
    struct DirEntryRecord {
        u32 offset{};
        u16 streamingSize{};
        u16 size{};
    };
    struct DirectoryEntry {
        explicit DirectoryEntry(FileStream& io);
        explicit DirectoryEntry(EntryStatus& status, const std::filesystem::path& path);

        DirEntryRecord info;
        std::string name;

        bool operator <(const DirectoryEntry& compare) const {
            return info.offset < compare.info.offset;
        }
    };
    struct ImgHeader {
        std::array<char, 4> version;
        u32 itemCount;
    };

    constexpr auto structureSize{sizeof(DirEntryRecord)};
    constexpr auto entrySize{structureSize + 24};
}
