#pragma once
#include <vector>
#include <array>
#include <types.h>
namespace criterion::dff {
    class Extension {
    public:
        Extension() = default;
        explicit Extension(FileStream& stream);

        ChunkHeader header;

        std::vector<std::array<char, 0x1a>> couplers;
    };
}
