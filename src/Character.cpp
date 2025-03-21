#include "Character.hpp"
#include "Util/Image.hpp"


Character::Character(const std::string& ImagePath) {
    SetImage(ImagePath);

    ResetPosition();
}

void Character::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;

    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

bool Character::IfCollides(const std::shared_ptr<Character>& other) const {
    glm::vec2 pos1 = this->GetPosition();
    glm::vec2 pos2 = other->GetPosition();

    float distance = glm::length(pos1 - pos2);
    return distance < 50.0f;
}