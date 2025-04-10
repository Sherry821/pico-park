#include "Camera.hpp"
#include "Character.hpp"
#include "Util/Logger.hpp"
#include <algorithm>
#include <cmath>
#include <cfloat>

Camera::Camera(float viewWidth, float viewHeight)
    : m_Position(0.0f, 0.0f)
    , m_ViewWidth(viewWidth)
    , m_ViewHeight(viewHeight)
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


void Camera::SetBoundaries(float left, float right, float top, float bottom) {
    m_LeftBoundary = left;
    m_RightBoundary = right;
    m_TopBoundary = top;
    m_BottomBoundary = bottom;

    LOG_INFO("Camera boundaries set to: L={}, R={}, T={}, B={}", left, right, top, bottom);
}

glm::vec2 Camera::WorldToScreenPosition(const glm::vec2& worldPos) const {
    // 世界座標轉換為螢幕座標
    return worldPos - m_Position;
}

glm::vec2 Camera::ScreenToWorldPosition(const glm::vec2& screenPos) const {
    // 螢幕座標轉換為世界座標
    return screenPos + m_Position;
}

glm::vec2 Camera::CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const {
    glm::vec2 newPos = characterPos;

    // 檢查並修正角色位置，確保不會超出地圖邊界
    float halfWidth = characterSize.x * 0.5f;
    float halfHeight = characterSize.y * 0.5f;

    newPos.x = std::clamp(newPos.x, m_LeftBoundary + halfWidth, m_RightBoundary - halfWidth);
    newPos.y = std::clamp(newPos.y, m_BottomBoundary + halfHeight, m_TopBoundary - halfHeight);

    return newPos;
}