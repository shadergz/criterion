#pragma once
#include <vector>

#include <types.h>
namespace criterion::dds {
    class DdsBacking {
    public:
        static void getBytes(std::vector<u8>& output, const std::vector<u8>& data,
            std::span<u8> four, u32 width, u32 height, std::span<u8> mipMap);

        static void stripDdsHeader(std::vector<u8>& ddsData);
    };
}