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

    m_Camera->Update(m_pico1, m_pico2);

    // 相機跟隨與角色互動部分
// In AppUpdate.cpp - replace or modify the existing camera follow code
if (m_Phase == Phase::STAGE_THREE) {
    // Make sure map tiles are visible in this phase
    static bool firstTimeInPhase3 = true;
    if (firstTimeInPhase3) {
        for (auto& tile : m_MapManager->GetMapTiles()) {
            tile->SetVisible(true);
        }
        m_pico1->SetVisible(true);
        m_pico2->SetVisible(true);
        firstTimeInPhase3 = false;
    }

    // Update character positions based on input
    glm::vec2 newPosition1 = m_pico1->GetPosition();
    glm::vec2 newPosition2 = m_pico2->GetPosition();

    // Handle character 1 movement
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        newPosition1.x -= 5.0f;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        newPosition1.x += 5.0f;
    }

    // Handle character 2 movement
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        newPosition2.x -= 5.0f;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        newPosition2.x += 5.0f;
    }

    // Apply character movement with boundary checks
    float mapLeft, mapRight, mapTop, mapBottom;
    m_MapManager->GetMapBoundaries(mapLeft, mapRight, mapTop, mapBottom);

    // Apply boundary constraints for character 1
    float width1 = 25.0f;  // Approximate width
    float height1 = 50.0f; // Approximate height
    newPosition1.x = glm::clamp(newPosition1.x, mapLeft + width1 * 0.5f, mapRight - width1 * 0.5f);
    newPosition1.y = glm::clamp(newPosition1.y, mapBottom + height1 * 0.5f, mapTop - height1 * 0.5f);

    // Apply boundary constraints for character 2
    float width2 = 25.0f;
    float height2 = 50.0f;
    newPosition2.x = glm::clamp(newPosition2.x, mapLeft + width2 * 0.5f, mapRight - width2 * 0.5f);
    newPosition2.y = glm::clamp(newPosition2.y, mapBottom + height2 * 0.5f, mapTop - height2 * 0.5f);

    // Update character positions
    m_pico1->SetPosition(newPosition1);
    m_pico2->SetPosition(newPosition2);

    // Update camera to follow characters
    m_Camera->Update(m_pico1, m_pico2);

    // Update map tiles based on camera position
    for (auto& tile : m_MapManager->GetMapTiles()) {
        glm::vec2 worldPos = tile->GetPosition();
        glm::vec2 screenPos = m_Camera->WorldToScreenPosition(worldPos);
        tile->SetPosition(screenPos);

        // Only show tiles that are within the camera view (with some margin)
        float margin = 100.0f;
        bool isVisible = (std::abs(screenPos.x) < m_Camera->GetViewWidth() * 0.5f + margin &&
                          std::abs(screenPos.y) < m_Camera->GetViewHeight() * 0.5f + margin);
        tile->SetVisible(isVisible);
    }
}

    // In AppUpdate.cpp - Add this in the Update method where character movement is handled

// Check if characters are out of bounds and output a signal
bool pico1OutOfBounds = m_Camera->IsCharacterOutOfBounds(m_pico1);
bool pico2OutOfBounds = m_Camera->IsCharacterOutOfBounds(m_pico2);

if (pico1OutOfBounds) {
    LOG_DEBUG("Pico1 is out of bounds! Position: ({}, {})",
                m_pico1->GetPosition().x, m_pico1->GetPosition().y);
}

if (pico2OutOfBounds) {
    LOG_DEBUG("Pico2 is out of bounds! Position: ({}, {})",
                m_pico2->GetPosition().x, m_pico2->GetPosition().y);
}

// If either character is out of bounds, set a visual indicator or play a sound
if (pico1OutOfBounds || pico2OutOfBounds) {
    // You could add a visual indicator here, like changing character color
    // Or set a flag to be used elsewhere in your code
    m_IsCharacterOutOfBounds = true;

    // You could also force characters to stay within bounds
    if (pico1OutOfBounds) {
        // Adjust pico1's position to keep in bounds
        glm::vec2 pos = m_pico1->GetPosition();
        glm::vec2 screenPos = m_Camera->WorldToScreenPosition(pos);

        // Calculate the adjustment needed
        float halfWidth = m_Camera->GetViewWidth() * 0.5f;
        float halfHeight = m_Camera->GetViewHeight() * 0.5f;

        if (screenPos.x < -halfWidth) screenPos.x = -halfWidth;
        if (screenPos.x > halfWidth) screenPos.x = halfWidth;
        if (screenPos.y < -halfHeight) screenPos.y = -halfHeight;
        if (screenPos.y > halfHeight) screenPos.y = halfHeight;

        // Convert back to world position and set
        m_pico1->SetPosition(m_Camera->ScreenToWorldPosition(screenPos));
    }

    if (pico2OutOfBounds) {
        // Similar adjustment for pico2
        glm::vec2 pos = m_pico2->GetPosition();
        glm::vec2 screenPos = m_Camera->WorldToScreenPosition(pos);

        float halfWidth = m_Camera->GetViewWidth() * 0.5f;
        float halfHeight = m_Camera->GetViewHeight() * 0.5f;

        if (screenPos.x < -halfWidth) screenPos.x = -halfWidth;
        if (screenPos.x > halfWidth) screenPos.x = halfWidth;
        if (screenPos.y < -halfHeight) screenPos.y = -halfHeight;
        if (screenPos.y > halfHeight) screenPos.y = halfHeight;

        m_pico2->SetPosition(m_Camera->ScreenToWorldPosition(screenPos));
    }
} else {
    m_IsCharacterOutOfBounds = false;
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