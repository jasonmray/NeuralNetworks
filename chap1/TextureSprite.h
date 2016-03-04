
#ifndef J_TEXTURESPRITE_H
#define J_TEXTURESPRITE_H

#include "Jargon/ScopedPointer.h"

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>



namespace J{

class  TextureSprite : public sf::Drawable, public sf::Transformable
{
public:

	TextureSprite();
    explicit TextureSprite(sf::Texture * texture);
    TextureSprite(sf::Texture * texture, const sf::IntRect& rectangle);

    void setTexture(sf::Texture * texture, bool resetRect = false);
    void setTextureRect(const sf::IntRect& rectangle);
    void setColor(const sf::Color& color);
    const sf::Texture * getTexture() const;
    const sf::IntRect& getTextureRect() const;
    const sf::Color& getColor() const;
    sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void updatePositions();
    void updateTexCoords();

    sf::Vertex         m_vertices[4]; ///< Vertices defining the sprite's geometry
    Jargon::ScopedPointer<sf::Texture> m_texture;     ///< Texture of the sprite
    sf::IntRect        m_textureRect; ///< Rectangle defining the area of the source texture to display
};


}

#endif
