#include <cstring>
#include <dff/geometry.h>

namespace criterion::dff {
    ImplicitString::ImplicitString(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));
        stream.read(&buffer[0], sizeof(header));

        std::memcpy(&header, &buffer[0], sizeof(header));
        // Due to the null byte
        buffer.resize(header.size);
        stream.read(&buffer[0], buffer.size());

        string = std::string(&buffer[0], std::strlen(&buffer[0]));
    }

    Texture::Texture(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));
        stream.read(&buffer[0], sizeof(header));

        std::memcpy(&header, &buffer[0], sizeof(header));

        name = ImplicitString(stream);
        mask = ImplicitString(stream);
    }

    Material::Material(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));
        stream.read(&buffer[0], sizeof(header));

        std::memcpy(&header, &buffer[0], sizeof(header));
        if (header.textured) {
            texture = Texture(stream);
        }

        extension = Extension(stream);
    }

    MaterialList::MaterialList(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));
        stream.read(&buffer[0], sizeof(header));
        std::memcpy(&header, &buffer[0], sizeof(header));

        for (u32 index{}; index < header.materialCount; index++) {
            stream.read(&buffer[0], sizeof(4));
        }
        for (u32 index{}; index < header.materialCount; index++) {
            materials.emplace_back(stream);
        }
    }

    // https://gtamods.com/wiki/RpGeometry
    Geometry::Geometry(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));
        const auto position{stream.tellg()};
        stream.read(&buffer[0], sizeof(header));

        std::memcpy(&header, &buffer[0], sizeof(header));

        if (header.flags & 0x8) {
            for (u32 index{}; index < header.vertexCount; index++) {
                stream.read(&buffer[0], 4);
                colors.emplace_back(
                    static_cast<float>(buffer[0]),
                    static_cast<float>(buffer[1]),
                    static_cast<float>(buffer[2]),
                    static_cast<float>(buffer[3]));
            }
        }
        if (header.flags & 0xf || header.flags & 0x80) {
            for (u32 index{}; index < header.vertexCount; index++) {
                stream.read(&buffer[0], sizeof(glm::vec2));
                static_assert(sizeof(glm::vec2) == sizeof(float) * 2);
                textCoords.emplace_back(
                    *reinterpret_cast<float*>(&buffer[0]),
                    *reinterpret_cast<float*>(&buffer[4]));
            }
        }

        RenderTriangles triangle{};
        for (u32 index{}; index < header.trianglesCount; index++) {
            stream.read(&buffer[0], sizeof(u16) * 4);
            triangle.xyz.x = *reinterpret_cast<u16*>(&buffer[0]);
            triangle.xyz.y = *reinterpret_cast<u16*>(&buffer[2]);
            triangle.xyz.z = *reinterpret_cast<u16*>(&buffer[4]);

            triangle.materialIndex = *reinterpret_cast<u16*>(&buffer[6]);

            triangles.push_back(triangle);
        }

        stream.seekg(position, std::ios::beg);
        stream.seekg(header.structure.size + 24, std::ios::cur);

        materialPack = MaterialList(stream);
    }
}
