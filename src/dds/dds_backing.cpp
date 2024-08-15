#include <cstring>
#include <dds/dds_backing.h>

namespace criterion::dds {
    void DdsBacking::getBytes(std::vector<u8>& output, const std::vector<u8>& data, const std::span<u8> four,
        u32 width, u32 height, const std::span<u8> mipMap) {

        const std::span widthBytes{reinterpret_cast<u8*>(&width), 4};
        const std::span heightBytes{reinterpret_cast<u8*>(&height), 4};

        u32 count{};
        u32 unknown{};
        const std::span mipMapCountBytes{reinterpret_cast<u8*>(&count), 4};
        const std::span unknownSet{reinterpret_cast<u8*>(&unknown), 4};

        constexpr u8 mipMapFlagByte{};
        if (!mipMap.empty()) {

        }

        std::vector<u8> hdr{'D', 'D', 'S', ' ', 0x7c, 0x00, 0x00, 0x00, 0x07, 0x10};
        hdr.resize(128);
        hdr[10] = mipMapFlagByte;
        std::memcpy(&hdr[12], &heightBytes[0], sizeof(height));
        std::memcpy(&hdr[16], &widthBytes[0], sizeof(width));
        std::memcpy(&hdr[28], &mipMapCountBytes[0], sizeof(count));

        hdr[76] = 0x20;
        hdr[80] = 0x04;
        std::memcpy(&hdr[84], &four[0], sizeof(i32));

        std::memcpy(&hdr[108], &unknownSet[0], sizeof(unknown));

        output.clear();
        output.resize(hdr.size() + data.size());

        std::memcpy(&output[0], &hdr[0], hdr.size());
        std::memcpy(&output[128], &data[0], data.size());
    }

    void DdsBacking::stripDdsHeader(std::vector<u8>& ddsData) {
        ddsData.erase(ddsData.begin(), ddsData.begin() + 128);
    }
}
