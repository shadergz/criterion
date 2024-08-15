#pragma once

#include <types.h>
#include <txd/textures_container.h>
namespace criterion::txd {
    class TxdStream {
    public:
        explicit TxdStream(FileStream& txd);
        void extract(bool withPngs, const std::filesystem::path& output) const;

    private:
        ChunkHeader header{};
        TexturesContainer textures{};
    };
}