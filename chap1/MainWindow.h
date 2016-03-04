
#ifndef J_MAINWINDOW_H
#define J_MAINWINDOW_H

#include "Window.h"
#include "TextureSprite.h"

#include "tbb/spin_mutex.h"
#include "SFML/Graphics/Text.hpp"
#include <memory>


namespace J{

	class MainWindow : public Window{
		public:
			MainWindow();
			~MainWindow();

			void draw(sf::RenderWindow & window);

			void updateGraphStructure(const std::vector<std::shared_ptr<sf::Drawable> > & elements);
			void updateImage(const std::shared_ptr<TextureSprite> & imageSprite);
			void setWeightedImageSprite(size_t index, const std::shared_ptr<TextureSprite> & imageSprite);
			void setOutputImageSprite(size_t index, const std::shared_ptr<TextureSprite> & imageSprite);
			void updateLabel(std::shared_ptr<sf::Text> label);

		private:
			typedef std::shared_ptr<sf::Drawable> DrawablePtr;
			std::shared_ptr<std::vector<DrawablePtr> > m_graphStructure;
			std::shared_ptr<TextureSprite> m_imageSprite;
			std::vector<std::shared_ptr<TextureSprite> > m_weightedImageSprites;
			std::vector<std::shared_ptr<TextureSprite> > m_outputImageSprites;
			std::shared_ptr<sf::Text> m_label;
			tbb::spin_mutex m_elementsLock;
	};

}

#endif
