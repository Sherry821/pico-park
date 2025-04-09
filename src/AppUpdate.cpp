#include <iostream>

#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void App::Update() {

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 簡化 Enter 按鍵處理
    //if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
    //    ValidTask();
    //}

    if (m_EnterDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            ValidTask();
       }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);

    // === 🔽 碰撞檢測 🔽 ===
    if (m_Giraffe->IfCollides(m_Chest)) {
        m_Chest->SetVisible(false);  // 隱藏寶箱
    }
    // === 🔼 碰撞檢測結束 🔼 ===

    // === 🔽 加入角色移動邏輯 🔽 ===
    //float deltaTime = Util::Time::GetDeltaTime();
    const float gravity = -20.0f;     // 模擬重力
    //const float maxJumpHeight = 80.0f;  // 最大跳躍高度
    //const float jumpForce = 60.0f;   // 跳躍力道
    const float groundLevel = -150.0f;  // 地面高度 (假設地面 y = 0)
    const float fallAcceleration = -2.0f; // 下墜加速

    //const float deltaTime = 0.016f;  // 每幀時間 (假設每秒 60FPS)

    // m_pico1 (WAD 控制)
    float speed1 = 5.0f;
    glm::vec2 newPosition1 = m_pico1->GetPosition();
    float velocityY1 = 0.0f;  // 垂直速度 (用於重力與跳躍)
    //bool isJumping1 = false; // 標記角色是否正在跳躍

    // m_pico2 (上下左右 控制)
    float speed2 = 5.0f;
    glm::vec2 newPosition2 = m_pico2->GetPosition();
    float velocityY2 = 0.0f;  // 垂直速度 (用於重力與跳躍)
    //bool isJumping2 = false;

    // ---- m_pico1 移動邏輯 (WAD 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::W) && newPosition1.y <= groundLevel) {
        m_pico1 -> Isjumping();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        newPosition1.x -= speed1;  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        newPosition1.x += speed1;  // 右移
    }
    // ---- m_pico2 移動邏輯 (上下左右 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::UP) && newPosition2.y <= groundLevel) {
        m_pico2 -> Isjumping();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        newPosition2.x -= speed2;  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        newPosition2.x += speed2;  // 右移
    }

// 在 App::Update() 中
// 使用您實際的 Phase 枚舉值，這裡假設是 STAGE_TWO
if (m_Phase == Phase::STAGE_THREE) { // 或者您實際使用的階段
    // 只在特定階段啟用相機跟隨和地圖顯示
    if (m_Camera && m_pico1 && m_pico2) {
        // 重要：保存角色的原始世界座標
        glm::vec2 worldPos1 = m_pico1->GetPosition();
        glm::vec2 worldPos2 = m_pico2->GetPosition();

        // 打印調試信息
        std::cout << "Before camera update - Pico1: (" << worldPos1.x << ", " << worldPos1.y
                  << "), Pico2: (" << worldPos2.x << ", " << worldPos2.y << ")" << std::endl;

        // 更新相機位置
        m_Camera->Update(m_pico1, m_pico2);

        // 獲取更新後的相機位置
        glm::vec2 cameraPos = m_Camera->GetPosition();
        std::cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ")" << std::endl;

        // 更新地圖磚塊的位置（如果有地圖管理器）
        if (m_MapManager) {
            // 顯示所有地圖磚塊
            for (auto& tile : m_MapManager->GetMapTiles()) {
                tile->SetVisible(true);

                // 獲取磚塊的世界座標
                glm::vec2 tileWorldPos = tile->GetPosition();

                // 轉換為螢幕座標
                glm::vec2 tileScreenPos = m_Camera->WorldToScreenPosition(tileWorldPos);

                // 更新磚塊位置
                tile->SetPosition(tileScreenPos);
            }
        }

        // 將角色位置轉換為螢幕座標
        glm::vec2 screenPos1 = m_Camera->WorldToScreenPosition(worldPos1);
        glm::vec2 screenPos2 = m_Camera->WorldToScreenPosition(worldPos2);

        // 打印調試信息
        std::cout << "After camera transform - Pico1 screen: (" << screenPos1.x << ", " << screenPos1.y
                  << "), Pico2 screen: (" << screenPos2.x << ", " << screenPos2.y << ")" << std::endl;

        // 更新角色位置
        m_pico1->SetPosition(screenPos1);
        m_pico2->SetPosition(screenPos2);
    }
} else {
    // 在其他階段隱藏地圖
    if (m_MapManager) {
        for (auto& tile : m_MapManager->GetMapTiles()) {
            tile->SetVisible(false);
        }
    }
}




    // ---- 重力與跳躍的更新邏輯 ----
    // if (isJumping1) {
    //     velocityY1 += gravity*0.2f;
    //     newPosition1.y += velocityY1;
    //     if (newPosition1.y - groundLevel >= maxJumpHeight) {
    //         velocityY1 = fallAcceleration; // 達到最大高度後，開始下墜
    //     }
    // }
    velocityY1 += gravity*0.2f;
    newPosition1.y += velocityY1;

    // if (isJumping2) {
    //     //LOG_DEBUG("IS JUMPING.");
    //     velocityY2 += gravity*0.2f; // 平滑減速上升
    //     newPosition2.y += velocityY2;
    //     if (newPosition2.y - groundLevel >= maxJumpHeight) {
    //         velocityY2 = fallAcceleration; // 達到最大高度後，開始下墜
    //     }
    // }
    velocityY2 += gravity * 0.2f;
    newPosition2.y += velocityY2;

    // 防止角色穿透地面
    if (newPosition1.y < groundLevel) {
        newPosition1.y = groundLevel;
        velocityY1 = 0.0f;
    }

    if (newPosition2.y < groundLevel) {
        newPosition2.y = groundLevel;
        velocityY2 = 0.0f;
    }
    // 設定角色新位置
    m_pico1->SetPosition(newPosition1);
    m_pico2->SetPosition(newPosition2);

    // === 🔼 角色移動邏輯結束 🔼 ===

    // === 🔽 讓長頸鹿碰到門，門就打開 🔽 ===
    if (m_Phase == Phase::STAGE_ONE) {
        for (const auto& door : m_Doors) {
            if (m_Giraffe->IfCollides(door)) {  // ✅ 碰撞檢測
                door->SetVisible(true);  // ✅ 門打開
                door->SetImage(GA_RESOURCE_DIR"/Image/Character/door_open.png");
            }
        }
    }
    m_pico1 -> Ismoving();
    m_pico2 -> Ismoving();
    m_Root.Update();
}