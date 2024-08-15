#pragma once

#include <list>
#include <types.h>
#include <txd/texture.h>
namespace criterion::txd {
    struct TextureTaggedData {
        std::string textureName;
        std::vector<u8> dds;
        std::string format;
    };

    class TexturesContainer {
    public:
        TexturesContainer() = default;
        explicit TexturesContainer(FileStream& stream);
        [[nodiscard]] std::vector<TextureTaggedData> collect() const;
    private:
        std::list<Texture> textures{};

        struct {
            ChunkHeader chunk;

            u16 count;
            u16 pad;
        } container{};
    };

}