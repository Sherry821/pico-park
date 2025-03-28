#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>

class Map {
public:
    // 從檔案載入地圖
    static std::vector<std::vector<int>> LoadMap(const std::string& filename);

    // 渲染地圖（如果需要的話）
    static void RenderMap(const std::vector<std::vector<int>>& matrix);
    static int CountFilledBlocks(const std::vector<std::vector<int>>& matrix);
    static void RenderMapToFile(const std::vector<std::vector<int>>& matrix, const std::string& outputFile);

    // ANSI escape codes for colors (如果需要的話)
    static const std::string ORANGE;
    static const std::string WHITE;
    static const std::string RESET;

private:
};

#endif // MAP_HPP