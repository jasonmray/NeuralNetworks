
#include "TextureSprite.h"


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdlib>

namespace J{

	////////////////////////////////////////////////////////////
	TextureSprite::TextureSprite() :
	m_texture    (NULL),
	m_textureRect()
	{
	}


	////////////////////////////////////////////////////////////
	TextureSprite::TextureSprite(sf::Texture * texture) :
	m_texture    (NULL),
	m_textureRect()
	{
		setTexture(texture);
	}


	////////////////////////////////////////////////////////////
	TextureSprite::TextureSprite(sf::Texture * texture, const sf::IntRect& rectangle) :
	m_texture    (NULL),
	m_textureRect()
	{
		setTexture(texture);
		setTextureRect(rectangle);
	}


	////////////////////////////////////////////////////////////
	void TextureSprite::setTexture(sf::Texture * texture, bool resetRect)
	{
		// Recompute the texture area if requested, or if there was no valid texture & rect before
		if (resetRect || (m_texture.isNull() && (m_textureRect == sf::IntRect())))
			setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));

		m_texture = texture;
	}


	////////////////////////////////////////////////////////////
	void TextureSprite::setTextureRect(const sf::IntRect& rectangle)
	{
		if (rectangle != m_textureRect)
		{
			m_textureRect = rectangle;
			updatePositions();
			updateTexCoords();
		}
	}


	////////////////////////////////////////////////////////////
	void TextureSprite::setColor(const sf::Color& color)
	{
		// Update the vertices' color
		m_vertices[0].color = color;
		m_vertices[1].color = color;
		m_vertices[2].color = color;
		m_vertices[3].color = color;
	}


	////////////////////////////////////////////////////////////
	const sf::Texture* TextureSprite::getTexture() const
	{
		return m_texture.getRaw();
	}


	////////////////////////////////////////////////////////////
	const sf::IntRect& TextureSprite::getTextureRect() const
	{
		return m_textureRect;
	}


	////////////////////////////////////////////////////////////
	const sf::Color& TextureSprite::getColor() const
	{
		return m_vertices[0].color;
	}

	////////////////////////////////////////////////////////////
	sf::FloatRect TextureSprite::getLocalBounds() const
	{
		float width = static_cast<float>(std::abs(m_textureRect.width));
		float height = static_cast<float>(std::abs(m_textureRect.height));

		return sf::FloatRect(0.f, 0.f, width, height);
	}


	////////////////////////////////////////////////////////////
	sf::FloatRect TextureSprite::getGlobalBounds() const
	{
		return getTransform().transformRect(getLocalBounds());
	}


	////////////////////////////////////////////////////////////
	void TextureSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_texture != NULL)
		{
			states.transform *= getTransform();
			states.texture = m_texture.getRaw();
			target.draw(m_vertices, 4, sf::TrianglesStrip, states);
		}
	}


	////////////////////////////////////////////////////////////
	void TextureSprite::updatePositions()
	{
		sf::FloatRect bounds = getLocalBounds();

		m_vertices[0].position = sf::Vector2f(0, 0);
		m_vertices[1].position = sf::Vector2f(0, bounds.height);
		m_vertices[2].position = sf::Vector2f(bounds.width, 0);
		m_vertices[3].position = sf::Vector2f(bounds.width, bounds.height);
	}


	////////////////////////////////////////////////////////////
	void TextureSprite::updateTexCoords()
	{
		float left   = static_cast<float>(m_textureRect.left);
		float right  = left + m_textureRect.width;
		float top    = static_cast<float>(m_textureRect.top);
		float bottom = top + m_textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(left, top);
		m_vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_vertices[2].texCoords = sf::Vector2f(right, top);
		m_vertices[3].texCoords = sf::Vector2f(right, bottom);
	}

}
