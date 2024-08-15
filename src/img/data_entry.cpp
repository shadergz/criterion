#include <img/data_entry.h>
namespace criterion::img {
    DataEntry::~DataEntry() {
        io.flush();
    }

    std::vector<u8> DataEntry::getData() const {
        const auto totalBytes{static_cast<u32>(super.info.streamingSize * sectorSize)};
        std::vector<u8> content(totalBytes);

        const auto bkpReadPtr{io.tellg()};
        const auto offset(super.info.offset * sectorSize);
        io.seekg(offset, std::ios::beg);

        io.read(reinterpret_cast<char*>(&content[0]), static_cast<std::streamsize>(content.size()));
        io.seekg(bkpReadPtr, std::ios::beg);

        return content;
    }

    void DataEntry::getData(FileStream& img, std::vector<u8>& content) const {
        const auto totalBytes{super.info.streamingSize * sectorSize};
        content.resize(totalBytes);

        std::fstream base{};
        bool shouldClose{};
        if (!io.is_open()) {
            shouldClose = true;
            base = std::fstream(io.path, std::ios::in | std::ios::binary);
            io.swap(base);
        }

        io.seekg(0);
        io.read(reinterpret_cast<char*>(&content[0]), totalBytes);
        io.seekg(-totalBytes, std::ios::cur);

        if (io.gcount() != totalBytes)
            throw std::runtime_error("Could not read data");
        const auto offset(super.info.offset * sectorSize);

        const auto position{img.tellp()};
        img.seekp(offset, std::ios::beg);
        const auto check{img.tellp()};
        img.write(reinterpret_cast<char*>(&content[0]), totalBytes);

        if (img.tellp() - check != totalBytes)
            throw std::runtime_error("Could not write data");

        img.seekp(position, std::ios::beg);

        if (shouldClose)
            io.close();
    }
}
