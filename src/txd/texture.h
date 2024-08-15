#pragma once

#include <types.h>
namespace criterion::txd {
    class TextureData {
    public:
        TextureData() = default;
        explicit TextureData(FileStream& stream);

        [[nodiscard]] std::vector<u8> getDds(bool withMipMaps = false) const;
        struct {
            ChunkHeader chunk;

            u32 version;
            u32 filterFlags;
            std::array<char, 32> name;
            std::array<char, 32> alphaName;
            u32 alphaFlags;
            u32 format;
            u16 width;
            u16 height;
            u8 depth;
            u8 mipMapCount;
            u8 texCodeType;
            u8 flags;
            union {
                std::array<u8, 256 * 4> palette;
                u32 sizeOptional;
            };
            u32 size;
        } dataHeader{};

        std::vector<u8> dataBuffer{};
        std::vector<u8> mipMap{};
    };

    class Texture {
    public:
        explicit Texture(FileStream& stream);
        TextureData data;

        [[nodiscard]] auto getData(const bool mipMap) const {
            return data.getDds(mipMap);
        }
        [[nodiscard]] auto getName() const {
            const auto fixed{std::string{&data.dataHeader.name[0], data.dataHeader.name.size()}};
            return fixed.substr(0, fixed.find('\0'));
        }
        [[nodiscard]] auto getFormat() const {
            return data.dataHeader.format;
        }
    private:
        ChunkHeader header{};
        std::vector<u8> extraInfo;
    };
}
