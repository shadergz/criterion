#include <vector>
#include <unordered_map>
#include <cstring>
#include <txd/textures_container.h>

namespace criterion::txd {
    TexturesContainer::TexturesContainer(FileStream& stream) {
        std::vector<char> buffer(sizeof(container));
        stream.read(&buffer[0], sizeof(container));

        std::memcpy(&container, &buffer[0], sizeof(container));

        for (u32 index{}; index < container.count ; ++index) {
            textures.emplace_back(stream);
        }
    }

    std::vector<TextureTaggedData> TexturesContainer::collect() const {
        std::vector<TextureTaggedData> result;
        std::unordered_map<u32, std::string> supportedDds;
        supportedDds.emplace(0, "S3TC DXT1");
        supportedDds.emplace(21, "RGBA32");
        supportedDds.emplace(22, "RGB32");

        for (const auto& texture : textures) {
            std::string format;
            const auto fmtKey{texture.getFormat()};
            if (!supportedDds.contains(fmtKey)) {
                std::stringstream ss;
                ss << fmtKey;
                format = ss.str();
            } else {
                format = supportedDds[fmtKey];
            }
            result.emplace_back(
                texture.getName(),
                texture.getData(true),
                format
            );
        }
        return result;
    }
}
