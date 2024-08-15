#include <cassert>
#include <vector>
#include <cstring>
#include <boost/mpl/assert.hpp>
#include <txd/texture.h>

#include <dds/dds_backing.h>
namespace criterion::txd {
    TextureData::TextureData(FileStream& stream) {
        std::vector<char> buffer(sizeof(dataHeader));
        stream.read(&buffer[0], sizeof(dataHeader));

        std::memcpy(&dataHeader, &buffer[0], sizeof(dataHeader));
        if (dataHeader.depth != 7) {
            dataHeader.size = dataHeader.sizeOptional;
            std::ranges::fill(dataHeader.palette, 0);

            stream.seekg(static_cast<std::streamoff>(-sizeof(dataHeader.palette)), std::ios::cur);
        }
        dataBuffer.resize(dataHeader.size);
        stream.read(reinterpret_cast<char*>(&dataBuffer[0]), dataHeader.size);

        const auto mapSize{dataHeader.mipMapCount - 1};
        if (mapSize == 0) {
            mipMap.resize(mapSize);
            stream.read(reinterpret_cast<char*>(&mipMap[0]), mapSize);
        }
    }

    std::vector<u8> TextureData::getDds(const bool withMipMaps) const {
        auto mapOf{withMipMaps ? mipMap : std::vector<u8>{}};
        std::vector<u8> result;

        std::array<u8, 4> format{};
        std::memcpy(&format[0], &dataHeader.format, 4);
        dds::DdsBacking::getBytes(result, dataBuffer, format, dataHeader.width, dataHeader.height, mapOf);

        return result;
    }

    Texture::Texture(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));
        stream.read(&buffer[0], sizeof(header));

        std::memcpy(&header, &buffer[0], sizeof(header));
        auto position{stream.tellg()};

        data = TextureData(stream);
        position += header.size;
        if (stream.tellg() > position) {
            throw std::runtime_error("Failed to read texture data");
        }
        ChunkHeader extra{};
        stream.read(&buffer[0], sizeof(extra));
        std::memcpy(&extra, &buffer[0], sizeof(extra));
        if (extra.type != 3)
            return;

        if (extra.size) {
            extraInfo.resize(extra.size);
            stream.read(reinterpret_cast<char*>(&extraInfo[0]), extra.size);
        }

    }
}
