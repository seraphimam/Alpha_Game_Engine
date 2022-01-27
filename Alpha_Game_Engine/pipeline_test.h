#pragma once

#include <string>
#include <vector>
#include "device.h"

struct PipelineConfigInfo{};

class MyPipeline {
public:
    MyPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

private:
    static std::vector<char> readFile(const std::string& filePath);

    void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
};