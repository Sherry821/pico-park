#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include "Util/GameObject.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::string& ImagePath);

    Character(const Character&) = delete;

    Character(Character&&) = delete;

    Character& operator=(const Character&) = delete;

    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void Isjumping();

    void Ismoving();

    void SetImage(const std::string& ImagePath);

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    bool IfCollides(const std::shared_ptr<Character>& other) const;

    bool IsStanding(const std::shared_ptr<Character>& character) const;

    glm::vec2 GetScaledSize() const {
        // 假設有一個原始大小
        glm::vec2 originalSize(100.0f, 100.0f); // 根據實際情況調整
        return originalSize * m_Transform.scale;
    }

    bool CheckTileCollision();




private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    int JumpVelocity = 0;
    std::string m_ImagePath;
};


#endif //CHARACTER_HPP