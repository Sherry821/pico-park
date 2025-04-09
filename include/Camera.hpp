#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include "glm/glm.hpp"
#include "Character.hpp"

class Camera {
public:
    Camera(float viewWidth, float viewHeight);

    // 更新相機位置，跟隨指定的角色
    void Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2);

    // 獲取相機位置
    glm::vec2 GetPosition() const { return m_Position; }

    // 設置相機位置
    void SetPosition(const glm::vec2& position) { m_Position = position; }

    // 將世界座標轉換為螢幕座標
    glm::vec2 WorldToScreenPosition(const glm::vec2& worldPos) const;

    // 將螢幕座標轉換為世界座標
    glm::vec2 ScreenToWorldPosition(const glm::vec2& screenPos) const;

    // 設置相機邊界
    void SetBoundaries(float left, float right, float top, float bottom);

    // 檢查角色是否在可視範圍內，如果不在則返回需要的調整量
    glm::vec2 CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const;

private:
    glm::vec2 m_Position; // 相機在世界中的位置
    float m_ViewWidth;    // 視野寬度
    float m_ViewHeight;   // 視野高度

    // 相機移動邊界
    float m_LeftBoundary;
    float m_RightBoundary;
    float m_TopBoundary;
    float m_BottomBoundary;

    // 相機平滑移動參數
    float m_SmoothFactor;
};

#endif // CAMERA_HPP
