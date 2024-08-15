#include <vector>
#include <cstring>
#include <print>

#include <IL/il.h>

#include <txd/txd.h>
#include <dds/dds_backing.h>
namespace criterion::txd {
    TxdStream::TxdStream(FileStream& txd) {
        std::fstream io{txd.path, std::ios::in | std::ios::binary};
        txd.swap(io);

        std::vector<char> buffer(sizeof(header));
        txd.read(&buffer[0], sizeof(header));

        std::memcpy(&header, &buffer[0], sizeof(header));

        textures = TexturesContainer(txd);
    }

    void TxdStream::extract(const bool withPngs, const std::filesystem::path& output) const {
        // ReSharper disable once CppUseStructuredBinding
        const std::filesystem::path png{output / "png"};
        if (!exists(png))
            create_directory(png);

        ilInit();
        const ILuint canvas{ilGenImage()};
        ilBindImage(canvas);

        const auto tagged{textures.collect()};
        for (const auto& [texture, dds, format] : tagged) {
            std::print("Decompressing the texture: {}\n", texture);

            std::filesystem::path path{output / (texture + ".dds")};
            FileStream io{path, std::ios::out | std::ios::binary};

            io.write(reinterpret_cast<const char*>(&dds[0]), dds.size());
            io.close();

            if (!withPngs || format.contains("DXT1")) {
                auto ddsCp{dds};
                dds::DdsBacking::stripDdsHeader(ddsCp);
                // S3TC DXT1
                continue;
            }
            ilLoadL(IL_DDS, &dds[0], dds.size());

            if (ilGetError() != IL_NO_ERROR)
                throw std::runtime_error(std::format("Could not load the DDS file: {}", ilGetError()));
            path = png / (texture + ".png");
            ilSave(IL_PNG, path.string().c_str());
        }

        ilDeleteImage(canvas);
    }
}
