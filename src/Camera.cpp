// Camera.cpp
#include "Camera.hpp"
#include "Character.hpp"
#include <algorithm>
#include <iostream> // 用於調試

Camera::Camera(float viewWidth, float viewHeight)
    : m_Position(0.0f, 0.0f)
    , m_ViewWidth(viewWidth)
    , m_ViewHeight(viewHeight)
    , m_LeftBoundary(-1000.0f)
    , m_RightBoundary(1000.0f)
    , m_TopBoundary(1000.0f)
    , m_BottomBoundary(-1000.0f)
    , m_SmoothFactor(0.1f)
{
    std::cout << "Camera initialized with view size: " << viewWidth << "x" << viewHeight << std::endl;
}

void Camera::SetBoundaries(float left, float right, float top, float bottom) {
    m_LeftBoundary = left;
    m_RightBoundary = right;
    m_TopBoundary = top;
    m_BottomBoundary = bottom;

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
    return worldPos - m_Position;
}

glm::vec2 Camera::ScreenToWorldPosition(const glm::vec2& screenPos) const {
    // 螢幕座標 -> 世界座標
    return screenPos + m_Position;
}

glm::vec2 Camera::CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const {
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
