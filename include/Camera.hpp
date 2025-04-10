<<<<<<< HEAD
// Camera.hpp
=======
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc
#pragma once
#include <glm/glm.hpp>
#include <memory>

class Character; // 前向宣告

class Camera {
public:
    Camera(float viewWidth, float viewHeight);

    void Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2);
    void SetBoundaries(float left, float right, float top, float bottom);

    glm::vec2 WorldToScreenPosition(const glm::vec2& worldPos) const;
    glm::vec2 ScreenToWorldPosition(const glm::vec2& screenPos) const;
    glm::vec2 CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const;

    const glm::vec2& GetPosition() const { return m_Position; }
<<<<<<< HEAD
=======
    float GetViewWidth() const { return m_ViewWidth; }
    float GetViewHeight() const { return m_ViewHeight; }
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc

private:
    glm::vec2 m_Position;
    float m_ViewWidth;
    float m_ViewHeight;
    float m_LeftBoundary;
    float m_RightBoundary;
    float m_TopBoundary;
    float m_BottomBoundary;
    float m_SmoothFactor;
<<<<<<< HEAD
};
=======
};
>>>>>>> 7fdaa699bf7b761ecafb95fd44c8ec57d1e973cc
