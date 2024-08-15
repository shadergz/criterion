#pragma once

#include <cstdint>
#include <fstream>
#include <filesystem>
namespace criterion {
    class FileStream final : public std::fstream {
    public:
        explicit FileStream(const std::filesystem::path& input, const std::ios::openmode mode) :
            std::fstream(input, mode), path(input) {}
        explicit FileStream(const std::filesystem::path& input) :
            path(input) {}

        std::filesystem::path path;
    };
    class TaggedStream final : public std::stringstream {
        public:
        explicit TaggedStream(const std::filesystem::path& input) :
            path(input) {}
        std::filesystem::path path;
    };

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using i32 = std::int32_t;

    enum CriterionRwType : u32 {
        String = 2,
        Texture = 6,
        Material = 7,
        MaterialList = 8,
        Clump = 0x10,
        Geometry = 0xf,

        Frame = 0x0253f2fe // Stores the name of a frame within a Frame List
    };
    // https://gta.fandom.com/wiki/RenderWare_binary_stream_file
    enum CriterionRwMarker {
        Gta3 = 0x00000310,
        Gta31 = 0x0800ffff,
        GtaVc = 0x0C02ffff,
        GtaVc1 = 0x1003ffff,
        GtaSa = 0x1803ffff,
    };
    struct ChunkHeader {
        CriterionRwType type;
        u32 size;
        CriterionRwMarker marker;
    };
}
