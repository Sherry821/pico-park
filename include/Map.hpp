#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <memory>

// 如果要移動 CreateMapTiles，需要這些頭文件
class Character;
namespace Util { class Renderer; }

class Map {
public:
    // 從檔案載入地圖
    static std::vector<std::vector<int>> LoadMap(const std::string& filename);

    // 渲染地圖到控制台（用於除錯）
    static void RenderMap(const std::vector<std::vector<int>>& matrix);

    // 計算地圖中的實心方塊數量
    static int CountFilledBlocks(const std::vector<std::vector<int>>& matrix);

    // 將地圖保存到文件
    static void RenderMapToFile(const std::vector<std::vector<int>>& matrix, const std::string& outputFile);

    // 根據地圖數據創建實際的遊戲磚塊
    static void CreateMapTiles(std::vector<std::shared_ptr<Character>>& mapTiles,
                               Util::Renderer& root,
                               const std::vector<std::vector<int>>& map);

};

#endif // MAP_HPP