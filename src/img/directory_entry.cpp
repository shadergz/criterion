#include <cstring>

#include <img/directory_entry.h>
namespace criterion::img {
    DirectoryEntry::DirectoryEntry(FileStream& io) {
        std::array<char, entrySize> buffer{};
        io.read(&buffer[0], buffer.size());

        std::memcpy(&info, &buffer[0], structureSize);
        name = std::string(&buffer[structureSize]);
        name = name.substr(0, name.find(' '));
    }

    DirectoryEntry::DirectoryEntry(EntryStatus& status, const std::filesystem::path& path) {
        const auto requestSize{static_cast<u32>(file_size(path))};
        status.requiredSize += requestSize;
        info.size = {};
        info.streamingSize = static_cast<u16>(requestSize / sectorSize);

        info.offset = status.offset;
        status.offset += info.streamingSize;

        name = path.filename();
    }
}
