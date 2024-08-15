#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <process.h>
namespace criterion {
    extern "C" i32 main(const i32 argc, char** argv) {
        namespace po = boost::program_options;
        po::options_description options("Criterion Options");

        options.add_options()
            ("input", po::value<std::string>(&input))
            ("output", po::value<std::string>(&output))
            ("mode", po::value<std::string>(&mode), "")
            ("with", po::value<std::string>(&with))
            ("by", po::value<std::string>(&by))
            ("operation", po::value<std::string>(&operation), "");

        Process executor;
        po::variables_map vm;
        store(parse_command_line(argc, argv, options), vm);
        vm.notify();

        if (input.empty())
            throw std::runtime_error("");

        std::vector<std::string> inputFiles;
        split(inputFiles, input, boost::is_any_of(",;+"));

        std::list<FileStream> inputStreams;
        std::vector<std::filesystem::path> inputDirs;

        bool dirMode{};
        const auto desiredExtension{"." + mode};

        for (const auto& file : inputFiles) {
            // ReSharper disable once CppTooWideScopeInitStatement
            const std::filesystem::path inputPath{input};
            if (is_directory(inputPath) || inputPath.extension().empty())
                dirMode = true;

            if (dirMode)
                if (!is_directory(inputPath))
                    throw std::runtime_error("");

            if (!dirMode) {
                if (input.ends_with(desiredExtension))
                    inputStreams.emplace_back(file, std::ios::binary | std::ios::in);
            } else {
                inputDirs.emplace_back(inputPath);
            }
        }
        if (output.empty())
            output = "output-files";
        if (!std::filesystem::exists(output))
            std::filesystem::create_directory(output);

        if (mode == "img") {
            executor.imageFiles(inputDirs, std::move(inputStreams));
        }
        if (mode == "txd") {
            executor.txdFiles(std::move(inputStreams));
        }
        if (mode == "dff") {
            executor.dffFiles(std::move(inputStreams));
        }
        return {};
    }
}
