#pragma once

#include <types.h>
#include <dff/clump.h>
namespace criterion::dff {

    class DffStream {
    public:
        explicit DffStream(FileStream& stream);
        Clump clump;
        ChunkHeader header{};
    };
}