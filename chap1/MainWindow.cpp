
#include "MainWindow.h"

namespace J{

	MainWindow::MainWindow():
		Window(sf::VideoMode(900, 700), "Neural Net", 200, 5)
	{
	}

	MainWindow::~MainWindow(){
	}


	void MainWindow::updateGraphStructure(const std::vector<std::shared_ptr<sf::Drawable> > & elements){
		std::shared_ptr<std::vector<DrawablePtr> > elementListCopy(new std::vector<DrawablePtr>(elements));

		{
			tbb::spin_mutex::scoped_lock lock(m_elementsLock);
			m_graphStructure = elementListCopy;
		}
	}

	void MainWindow::updateImage(const std::shared_ptr<TextureSprite> & imageSprite){
		tbb::spin_mutex::scoped_lock lock(m_elementsLock);
		m_imageSprite = imageSprite;
	}

	void MainWindow::setWeightedImageSprite(size_t index, const std::shared_ptr<TextureSprite> & imageSprite){
		if( index >= m_weightedImageSprites.size() ){
			m_weightedImageSprites.resize(index+1);
		}
		m_weightedImageSprites[index] = imageSprite;
	}

	void MainWindow::setOutputImageSprite(size_t index, const std::shared_ptr<TextureSprite> & imageSprite){
		if( index >= m_outputImageSprites.size() ){
			m_outputImageSprites.resize(index+1);
		}
		m_outputImageSprites[index] = imageSprite;
	}

	void MainWindow::updateLabel(std::shared_ptr<sf::Text> label){
		m_label = label;
	}

	void MainWindow::draw(sf::RenderWindow & window){
		std::shared_ptr<std::vector<DrawablePtr> > elementListRef;
		std::shared_ptr<TextureSprite> imageSprite;
		std::vector<std::shared_ptr<TextureSprite> > weightedImageSprites;
		std::vector<std::shared_ptr<TextureSprite> > outputImageSprites;
		std::shared_ptr<sf::Text> label;

		{
			tbb::spin_mutex::scoped_lock lock(m_elementsLock);
			elementListRef = m_graphStructure;
			imageSprite = m_imageSprite;
			weightedImageSprites = m_weightedImageSprites;
			outputImageSprites = m_outputImageSprites;
			label = m_label;
		}

		if( elementListRef != NULL ){
			for( auto & elementRef : *elementListRef ){
				window.draw(*elementRef);
			}
		}
		if( imageSprite != NULL ){
			window.draw(*imageSprite);
		}
		for( auto & sprite : weightedImageSprites ){
			if( sprite != NULL ){
				window.draw(*sprite);
			}
		}
		for( auto & sprite : outputImageSprites ){
			if( sprite != NULL ){
				window.draw(*sprite);
			}
		}
		if( label != NULL ){
			window.draw(*label);
		}
	}
}
