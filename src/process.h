#pragma once

#include <img/img.h>
#include <txd/txd.h>
#include <dff/dff.h>
namespace criterion {
    class Process {
    public:
        Process() = default;
        void imageFiles(const std::vector<std::filesystem::path>& inputDirs,
            std::list<FileStream>&& imgStreams);
        void txdFiles(std::list<FileStream>&& txdStreams);

        void dffFiles(std::list<FileStream>&& dffStreams);
    private:
        std::unique_ptr<img::ImgStream> image;
        std::unique_ptr<txd::TxdStream> txd;
        std::unique_ptr<dff::DffStream> dff;

        std::vector<std::stringstream> streams;
    };

    inline std::string input{}, output{};
    inline std::string mode{};
    inline std::string operation{};
    inline std::string with{}, by{};
}
