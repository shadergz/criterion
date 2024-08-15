#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include <types.h>
#include <dff/extension.h>
namespace criterion::dff {
    class ImplicitString {
    public:
        ImplicitString() = default;
        explicit ImplicitString(FileStream& stream);

    private:
        ChunkHeader header;
        std::string string;
    };
    class Texture {
        public:
        Texture() = default;
        explicit Texture(FileStream& stream);
    private:
        struct {
            ChunkHeader chunk;
            ChunkHeader structure;
            u8 filterMode;
            u8 textCoord;
            u16 pad;
        } header;
        ImplicitString name;
        ImplicitString mask;
    };
    class Material {
        public:
        Material() = default;
        explicit Material(FileStream& stream);
    private:
        struct {
            ChunkHeader header;
            ChunkHeader structure;
            u32 flags;
            struct {
                u8 r, g, b, a;
            } color;
            u32 pad;
            u32 textured;
            float ambient;
            float specular;
            float diffuse;
        } header;

        Extension extension;
        Texture texture;
    };
    class MaterialList {
        public:
        MaterialList() = default;
        explicit MaterialList(FileStream& stream);
    private:
        struct {
            ChunkHeader chunk;
            ChunkHeader structure;
            u32 materialCount;
        } header;

        std::list<Material> materials;
    };

    class Geometry {
    public:
        explicit Geometry(FileStream& stream);
    private:
        struct {
            ChunkHeader chunk;
            ChunkHeader structure;
            u32 flags;
            u32 trianglesCount;
            u32 vertexCount;
            u32 morphTargetCount;
        } header;
        struct RenderTriangles {
            glm::vec3 xyz;
            u16 materialIndex;
        };

        std::vector<glm::vec4> colors;
        std::vector<glm::vec2> textCoords;
        std::vector<RenderTriangles> triangles;

        MaterialList materialPack;
    };
}
