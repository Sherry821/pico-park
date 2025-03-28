#include "App.hpp"
#include "./Util/GameObject.hpp"
#include "Util/Logger.hpp"
#include "Map.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // 玩家 1（PICO1）
    m_pico1 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/pico_stand1.png");
    m_pico1->m_Transform.translation = {-100.0f, -155.5f};
    m_pico1->SetZIndex(50);
    m_pico1->SetVisible(false);
    m_pico1->m_Transform.scale = glm::vec2(0.25f, 0.25f);  // 放大 0.25 倍
    m_Root.AddChild(m_pico1);

    // 玩家 2（PICO2）
    m_pico2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/pico_stand2.png");
    m_pico2->m_Transform.translation = {50.0f, -140.5f};
    m_pico2->SetZIndex(50);
    m_pico2->SetVisible(false);
    m_pico2->m_Transform.scale = glm::vec2(0.5f, 0.5f);  // 放大 0.5 倍
    m_Root.AddChild(m_pico2);

    m_Giraffe = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/giraffe.png");
    m_Giraffe->SetPosition({-112.5f, -140.5f});
    m_Giraffe->SetVisible(false);
    m_Giraffe->SetZIndex(50);
    m_Root.AddChild(m_Giraffe);


    m_Chest = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/chest.png");
    m_Chest->SetZIndex(5);
    m_Chest->SetPosition({197.5f, -3.5f});
    m_Chest->SetVisible(false);
    m_Root.AddChild(m_Chest);

    // 使用絕對路徑或更靈活的相對路徑
    std::string mapPath = GA_RESOURCE_DIR"/Map/first.txt";
    LOG_INFO("Attempting to load map from: {}", mapPath);
    m_Map = Map::LoadMap(mapPath);

    // 額外的除錯輸出
    if (m_Map.empty()) {
        LOG_ERROR("Map loading failed. Please check the file path and permissions.");
    } else {
        LOG_INFO("Map loaded successfully. Rows: {}", m_Map.size());
    }


    std::vector<std::string> beeImages;
    beeImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        beeImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/bee_" + std::to_string(i + 1) + ".png");
    }

    // 使用絕對路徑或更靈活的相對路徑
    std::string mapPath = "D:/2025-OOPL/PTSD-Practice-Giraffe-Adventure/Resources/Map/first.txt";
    LOG_INFO("Attempting to load map from: {}", mapPath);
    m_Map = Map::LoadMap(mapPath);

    // 額外的除錯輸出
    if (m_Map.empty()) {
        LOG_ERROR("Map loading failed. Please check the file path and permissions.");
    } else {
        LOG_INFO("Map loaded successfully. Rows: {}", m_Map.size());
    }

    m_Bee = std::make_shared<AnimatedCharacter>(beeImages);
    m_Bee->SetZIndex(5);
    m_Bee->SetVisible(false);
    m_Root.AddChild(m_Bee);

    for (int i = 0; i < 3; ++i) {
        m_Doors.push_back(std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png"));
        m_Doors[i]->SetZIndex(5);
        m_Doors[i]->SetPosition({205.5f - 80.f * i, -3.5f});
        m_Doors[i]->SetVisible(false);
        m_Root.AddChild(m_Doors[i]);
    }

    // TODO: The counting down ball for phase 6

    m_PRM = std::make_shared<PhaseResourceManger>();
    m_Root.AddChildren(m_PRM->GetChildren());


    m_CurrentState = State::UPDATE;
}