<<<<<<< HEAD
// Camera.cpp
#include "Camera.hpp"
#include "Character.hpp"
#include <algorithm>
#include <iostream> // 用於調試
=======
#include "Camera.hpp"
#include "Character.hpp"
#include "Util/Logger.hpp"
#include <algorithm>
#include <cmath>
#include <cfloat>
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc

Camera::Camera(float viewWidth, float viewHeight)
    : m_Position(0.0f, 0.0f)
    , m_ViewWidth(viewWidth)
    , m_ViewHeight(viewHeight)
<<<<<<< HEAD
    , m_LeftBoundary(-1000.0f)
    , m_RightBoundary(1000.0f)
    , m_TopBoundary(1000.0f)
    , m_BottomBoundary(-1000.0f)
    , m_SmoothFactor(0.1f)
{
    std::cout << "Camera initialized with view size: " << viewWidth << "x" << viewHeight << std::endl;
}

=======
    , m_LeftBoundary(-FLT_MAX)
    , m_RightBoundary(FLT_MAX)
    , m_TopBoundary(FLT_MAX)
    , m_BottomBoundary(-FLT_MAX)
    , m_SmoothFactor(0.1f)
{
    LOG_INFO("Camera initialized with view dimensions: {}x{}", viewWidth, viewHeight);
}

void Camera::Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2) {
    // Calculate target position based on available characters
    glm::vec2 targetPosition;
    bool hasCharacters = false;

    // Check if both characters exist and at least one is valid
    if (pico1 && pico2) {
        // Use midpoint between characters
        targetPosition = (pico1->GetPosition() + pico2->GetPosition()) * 0.5f;
        hasCharacters = true;
    } else if (pico1) {
        targetPosition = pico1->GetPosition();
        hasCharacters = true;
    } else if (pico2) {
        targetPosition = pico2->GetPosition();
        hasCharacters = true;
    }

    if (!hasCharacters) {
        return; // No characters to follow
    }

    // Apply boundary constraints
    float halfWidth = m_ViewWidth * 0.5f;
    float halfHeight = m_ViewHeight * 0.5f;

    // Constrain camera within map boundaries
    glm::vec2 constrainedTarget = targetPosition;

    // Only apply constraints if boundaries are properly set
    if (m_LeftBoundary + halfWidth < m_RightBoundary - halfWidth) {
        constrainedTarget.x = std::clamp(constrainedTarget.x,
                                         m_LeftBoundary + halfWidth,
                                         m_RightBoundary - halfWidth);
    }

    if (m_BottomBoundary + halfHeight < m_TopBoundary - halfHeight) {
        constrainedTarget.y = std::clamp(constrainedTarget.y,
                                         m_BottomBoundary + halfHeight,
                                         m_TopBoundary - halfHeight);
    }

    // Apply smooth camera movement
    m_Position = m_Position + (constrainedTarget - m_Position) * m_SmoothFactor;
}


>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc
void Camera::SetBoundaries(float left, float right, float top, float bottom) {
    m_LeftBoundary = left;
    m_RightBoundary = right;
    m_TopBoundary = top;
    m_BottomBoundary = bottom;
<<<<<<< HEAD

    std::cout << "Camera boundaries set to: L=" << left << ", R=" << right
              << ", T=" << top << ", B=" << bottom << std::endl;
}

void Camera::Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2) {
    if (!pico1 || !pico2) {
        std::cerr << "Error: Invalid character pointers in Camera::Update" << std::endl;
        return;
    }

    // 獲取角色的世界座標
    glm::vec2 pos1 = pico1->GetPosition();
    glm::vec2 pos2 = pico2->GetPosition();

    std::cout << "Character positions - Pico1: (" << pos1.x << ", " << pos1.y
              << "), Pico2: (" << pos2.x << ", " << pos2.y << ")" << std::endl;

    // 計算兩個角色的中點
    glm::vec2 midPoint = (pos1 + pos2) * 0.5f;

    // 設置相機目標位置
    glm::vec2 targetPos = midPoint;

    // 平滑移動相機
    m_Position = m_Position + (targetPos - m_Position) * m_SmoothFactor;

    // 確保相機不超出邊界
    m_Position.x = std::clamp(m_Position.x,
                             m_LeftBoundary + m_ViewWidth * 0.5f,
                             m_RightBoundary - m_ViewWidth * 0.5f);
    m_Position.y = std::clamp(m_Position.y,
                             m_BottomBoundary + m_ViewHeight * 0.5f,
                             m_TopBoundary - m_ViewHeight * 0.5f);

    std::cout << "Camera position updated to: (" << m_Position.x << ", " << m_Position.y << ")" << std::endl;
}

glm::vec2 Camera::WorldToScreenPosition(const glm::vec2& worldPos) const {
    // 世界座標 -> 螢幕座標
    // 螢幕中心是 (0,0)
=======
    
    LOG_INFO("Camera boundaries set to: L={}, R={}, T={}, B={}", left, right, top, bottom);
}

glm::vec2 Camera::WorldToScreenPosition(const glm::vec2& worldPos) const {
    // 世界座標轉換為螢幕座標
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc
    return worldPos - m_Position;
}

glm::vec2 Camera::ScreenToWorldPosition(const glm::vec2& screenPos) const {
<<<<<<< HEAD
    // 螢幕座標 -> 世界座標
=======
    // 螢幕座標轉換為世界座標
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc
    return screenPos + m_Position;
}

glm::vec2 Camera::CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const {
<<<<<<< HEAD
    glm::vec2 adjustment(0.0f, 0.0f);

    // 檢查世界邊界
    float halfSizeX = characterSize.x * 0.5f;
    float halfSizeY = characterSize.y * 0.5f;

    // 檢查左右邊界
    if (characterPos.x - halfSizeX < m_LeftBoundary) {
        adjustment.x = m_LeftBoundary - (characterPos.x - halfSizeX);
    } else if (characterPos.x + halfSizeX > m_RightBoundary) {
        adjustment.x = m_RightBoundary - (characterPos.x + halfSizeX);
    }

    // 檢查上下邊界
    if (characterPos.y + halfSizeY > m_TopBoundary) {
        adjustment.y = m_TopBoundary - (characterPos.y + halfSizeY);
    } else if (characterPos.y - halfSizeY < m_BottomBoundary) {
        adjustment.y = m_BottomBoundary - (characterPos.y - halfSizeY);
    }

    return adjustment;
}
=======
    glm::vec2 newPos = characterPos;
    
    // 檢查並修正角色位置，確保不會超出地圖邊界
    float halfWidth = characterSize.x * 0.5f;
    float halfHeight = characterSize.y * 0.5f;
    
    newPos.x = std::clamp(newPos.x, m_LeftBoundary + halfWidth, m_RightBoundary - halfWidth);
    newPos.y = std::clamp(newPos.y, m_BottomBoundary + halfHeight, m_TopBoundary - halfHeight);
    
    return newPos;
}
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc
