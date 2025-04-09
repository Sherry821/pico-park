#include "Camera.hpp"
#include "Util/Logger.hpp"

Camera::Camera(float viewWidth, float viewHeight)
    : m_Position(0.0f, 0.0f)
    , m_ViewWidth(viewWidth)
    , m_ViewHeight(viewHeight)
    , m_LeftBoundary(-10000.0f)
    , m_RightBoundary(10000.0f)
    , m_TopBoundary(10000.0f)
    , m_BottomBoundary(-10000.0f)
    , m_SmoothFactor(0.1f)
{
}

void Camera::Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2) {
    // 計算兩個角色的中點
    glm::vec2 pos1 = pico1->GetPosition();
    glm::vec2 pos2 = pico2->GetPosition();
    glm::vec2 midPoint = (pos1 + pos2) * 0.5f;

    // 如果角色距離過大，則優先跟隨走得更遠的角色
    float distance = glm::length(pos1 - pos2);
    if (distance > m_ViewWidth * 0.6f) {
        // 判斷哪個角色走得更遠（在x軸方向）
        if (std::abs(pos1.x) > std::abs(pos2.x)) {
            midPoint = pos1;
        } else {
            midPoint = pos2;
        }
    }

    // 平滑移動相機
    glm::vec2 targetPos = midPoint;
    m_Position = m_Position + (targetPos - m_Position) * m_SmoothFactor;

    // 確保相機不超出邊界
    m_Position.x = glm::clamp(m_Position.x, m_LeftBoundary + m_ViewWidth * 0.5f, m_RightBoundary - m_ViewWidth * 0.5f);
    m_Position.y = glm::clamp(m_Position.y, m_BottomBoundary + m_ViewHeight * 0.5f, m_TopBoundary - m_ViewHeight * 0.5f);
}

glm::vec2 Camera::WorldToScreenPosition(const glm::vec2& worldPos) const {
    return worldPos - m_Position;
}

glm::vec2 Camera::ScreenToWorldPosition(const glm::vec2& screenPos) const {
    return screenPos + m_Position;
}

void Camera::SetBoundaries(float left, float right, float top, float bottom) {
    m_LeftBoundary = left;
    m_RightBoundary = right;
    m_TopBoundary = top;
    m_BottomBoundary = bottom;
}

glm::vec2 Camera::CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const {
    glm::vec2 adjustment(0.0f, 0.0f);

    // 計算角色在螢幕上的位置
    glm::vec2 screenPos = WorldToScreenPosition(characterPos);

    // 檢查左右邊界
    float halfWidth = m_ViewWidth * 0.5f;
    if (screenPos.x - characterSize.x * 0.5f < -halfWidth) {
        adjustment.x = -halfWidth - (screenPos.x - characterSize.x * 0.5f);
    } else if (screenPos.x + characterSize.x * 0.5f > halfWidth) {
        adjustment.x = halfWidth - (screenPos.x + characterSize.x * 0.5f);
    }

    // 檢查上下邊界
    float halfHeight = m_ViewHeight * 0.5f;
    if (screenPos.y - characterSize.y * 0.5f < -halfHeight) {
        adjustment.y = -halfHeight - (screenPos.y - characterSize.y * 0.5f);
    } else if (screenPos.y + characterSize.y * 0.5f > halfHeight) {
        adjustment.y = halfHeight - (screenPos.y + characterSize.y * 0.5f);
    }

    return adjustment;
}
