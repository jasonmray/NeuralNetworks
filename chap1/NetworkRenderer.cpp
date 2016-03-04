
#include "NetworkRenderer.h"

#include "Jargon/Math/Utilities.h"
#include "Jargon/StringUtilities.h"

namespace J{

	sf::Color NetworkRenderer::OutlineColor(10,10,10);
	sf::Color NetworkRenderer::ConnectionColor(150,150,150);
	sf::Color NetworkRenderer::FillColor(252,252,252);
	sf::Color NetworkRenderer::HilightColor(255,240,175);
	float NetworkRenderer::DefaultCircleDrawRadius = 10.f;
	unsigned int NetworkRenderer::DefaultRenderMargin = 15;


	void NetworkRenderer::DefaultNodeStyler::styleNode(sf::CircleShape & shape, int column, int nodeIndex){
		shape.setFillColor(FillColor);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(OutlineColor);
	}


	NetworkRenderer::HilightingNodeStyler::HilightingNodeStyler(const std::vector<Eigen::MatrixXd> & deltaNablaWeights, const std::vector<Eigen::VectorXd> & deltaNablaBiases):
		m_deltaNablaWeights(deltaNablaWeights),
		m_deltaNablaBiases(deltaNablaBiases)
	{
	}

	void NetworkRenderer::HilightingNodeStyler::styleNode(sf::CircleShape & shape, int column, int nodeIndex){
		//if( m_deltaNablaBiases[column-1][nodeIndex] != 0 || m_deltaNablaWeights[column-1].row(nodeIndex).isZero() == false ){
		if( fabs(m_deltaNablaBiases[column-1][nodeIndex]) > 1.0e-5 ){
			shape.setFillColor(HilightColor);
		}else{
			shape.setFillColor(FillColor);
		}

		shape.setOutlineThickness(1);
		shape.setOutlineColor(OutlineColor);
	}


	NetworkRenderer::WeightedConnectionStyler::WeightedConnectionStyler(const std::vector<NetworkLayer> & layers){
		m_layerWeights.resize(layers.size());

		for( size_t i = 0; i < layers.size(); i++ ){
			const NetworkLayer & layer(layers[i]);
			const Eigen::MatrixXd & weights(layer.m_weights);
			m_layerWeights[i] = weights;
		}

		for(Eigen::MatrixXd & weights : m_layerWeights){
			double rangeMax = 0;
			double rangeMin = 0;

			for( Eigen::MatrixXd::Index i = 0; i < weights.rows(); ++i ){
				const Eigen::VectorXd & row(weights.row(i));
				rangeMax = std::max(row.maxCoeff(), rangeMax);
				rangeMin = std::min(row.minCoeff(), rangeMin);
			}

			for( Eigen::MatrixXd::Index i = 0; i < weights.rows(); ++i ){
				for( Eigen::MatrixXd::Index j = 0; j < weights.cols(); ++j ){
					weights.coeffRef(i,j) = Jargon::Math::rangeMap(weights.coeff(i,j), rangeMin, rangeMax, 0., 255.);
				}
			}
		}
	}

	void NetworkRenderer::WeightedConnectionStyler::styleConnection(size_t layerIndex, size_t nodeIndex, size_t inputNodeIndex, sf::Vertex & startVertex, sf::Vertex & endVertex){
		Eigen::VectorXd row( m_layerWeights[layerIndex].row(nodeIndex) );
		double weight = (255.0) * row[inputNodeIndex];

		endVertex.color.r = (sf::Uint8)weight;
		endVertex.color.g = (sf::Uint8)weight;
		endVertex.color.b = (sf::Uint8)weight;
		endVertex.color.a = 255;

		startVertex.color = endVertex.color;
	}



	NetworkRenderer::NetworkRenderer(Display & display, J::Network & network):
		m_display(display),
		m_network(network),
		m_circleDrawRadius(DefaultCircleDrawRadius),
		m_renderMargin(DefaultRenderMargin)
	{
		m_font.loadFromFile("../../data/Roboto-Medium.ttf");
		updateNetworkStructure();
	}

	NetworkRenderer::~NetworkRenderer(){
	}

	void NetworkRenderer::updateNetworkStructure(){
		DefaultNodeStyler nodeStyler;
		WeightedConnectionStyler connectionStyler(m_network.m_layers);
		updateNetworkStructure(nodeStyler, connectionStyler);
	}

	std::shared_ptr<sf::VertexArray> NetworkRenderer::generateConnectionLines(ConnectionStyler & connectionStyler){
		std::shared_ptr<sf::VertexArray> linesVertexes(new sf::VertexArray(sf::Lines));

		sf::Vector2f nodeCenterOffset(m_circleDrawRadius, m_circleDrawRadius);
		for( unsigned int col = 1; col < m_nodePositions.size()-1; col++ ){
			for( unsigned int node1 = 0; node1 < m_nodePositions[col].size(); node1++ ){
				sf::Vector2f nodePosition1(m_nodePositions[col][node1]);

				for( unsigned int node2 = 0; node2 < m_nodePositions[col+1].size(); node2++ ){
					sf::Vector2f nodePosition2(m_nodePositions[col+1][node2]);

					sf::Vertex startVertex(nodePosition1+nodeCenterOffset);
					sf::Vertex endVertex(nodePosition2+nodeCenterOffset);

					connectionStyler.styleConnection(col, node2, node1, startVertex, endVertex);

					linesVertexes->append(startVertex);
					linesVertexes->append(endVertex);
				}
			}
		}

		return linesVertexes;
	}

	void NetworkRenderer::calculateNodeParameters(){
		sf::RenderWindow & renderWindow(m_display.getMainWindow().getRenderWindow());
		sf::Vector2u canvasSize = renderWindow.getSize() - sf::Vector2u(m_renderMargin*2, m_renderMargin*2);

		unsigned int columnCount = (unsigned int)m_network.m_layerSizes.size();
		float columnWidth = canvasSize.x * 65.0f / 100.0f / columnCount;
		float columnSpacing = (canvasSize.x - columnWidth * columnCount) / (columnCount - 1);

		unsigned int maxRowCount = (unsigned int) *std::max_element(m_network.m_layerSizes.begin()+1, m_network.m_layerSizes.end());
		float minRowHeight = canvasSize.y * 90.0f / 100.0f / maxRowCount;
		float circleDiameter = std::min(columnWidth, minRowHeight);
		m_circleDrawRadius = std::max(circleDiameter / 2.f, 1.f);

		m_nodePositions.clear();
		m_nodePositions.resize(m_network.m_layerSizes.size());

		sf::Vector2f nodePosition;
		nodePosition.x = (float)m_renderMargin + columnWidth + columnSpacing;

		for( unsigned int col = 1; col < columnCount; col++ ){

			size_t columnNodeCount = m_network.m_layerSizes[col];
			float rowSpacing = (canvasSize.y - (circleDiameter * columnNodeCount)) / (columnNodeCount + 1);
			nodePosition.y = (float)m_renderMargin;

			for( size_t node = 0; node < columnNodeCount; node++ ){
				nodePosition.y += rowSpacing;
				m_nodePositions[col].push_back(nodePosition);
				nodePosition.y += circleDiameter;
			}

			nodePosition.x += columnWidth + columnSpacing;
		}
	}

	void NetworkRenderer::updateNetworkStructure(NodeStyler & nodeStyler, ConnectionStyler & connectionStyler){
		MainWindow & window(m_display.getMainWindow());

		calculateNodeParameters();

		std::shared_ptr<sf::VertexArray> linesVertexes(generateConnectionLines(connectionStyler));

		std::vector<std::shared_ptr<sf::Drawable> > elements;
		elements.push_back(linesVertexes);

		for( unsigned int col = 1; col < m_nodePositions.size(); col++ ){
			unsigned int nodeCount = (unsigned int)m_nodePositions[col].size();
			for( unsigned int node = 0; node < nodeCount; node++ ){
				sf::Vector2f nodePosition(m_nodePositions[col][node]);

				std::shared_ptr<sf::CircleShape> circle(new sf::CircleShape(m_circleDrawRadius));
				nodeStyler.styleNode(*circle, col, node);
				circle->setPosition(nodePosition);
				elements.push_back(circle);
			}
		}

		window.updateGraphStructure(elements);
	}

	void convertImageToRGBA8(const Mnist::ImageSet::Image & image, std::vector<sf::Uint8> & rgba8BytesOut){
		size_t rgba8Size = image.header.columnCount * image.header.rowCount * 4;
		rgba8BytesOut.resize(rgba8Size);

		size_t srcIndex = 0;
		size_t destIndex = 0;
		for( uint32_t row = 0; row < image.header.rowCount; row++ ){
			for( uint32_t col = 0; col < image.header.columnCount; col++ ){
				rgba8BytesOut[destIndex++] = 255-image.pixels[srcIndex];
				rgba8BytesOut[destIndex++] = 255-image.pixels[srcIndex];
				rgba8BytesOut[destIndex++] = 255-image.pixels[srcIndex];
				rgba8BytesOut[destIndex++] = 255;
				srcIndex++;
			}
		}
	}

	void convertVectorToRGBA8(const Eigen::VectorXd & v, size_t width, size_t height, std::vector<sf::Uint8> & rgba8BytesOut){
		size_t rgba8Size = width * height * 4;
		rgba8BytesOut.resize(rgba8Size);

		double rangeMax = v.maxCoeff();
		double rangeMin = v.minCoeff();

		size_t srcIndex = 0;
		size_t destIndex = 0;
		for( uint32_t row = 0; row < width; row++ ){
			for( uint32_t col = 0; col < height; col++ ){
				int val = (int)Jargon::Math::rangeMap(v[srcIndex], rangeMin, rangeMax, -255., 255.);
				rgba8BytesOut[destIndex++] = (uint8_t)(val < 0 ? -val : 0);
				rgba8BytesOut[destIndex++] = 0;
				rgba8BytesOut[destIndex++] = (uint8_t)(val > 0 ? val : 0);
				rgba8BytesOut[destIndex++] = 255;
				srcIndex++;
			}
		}
	}

	std::shared_ptr<TextureSprite> createSpriteFromImage(const Mnist::ImageSet::Image & image){
		sf::Texture * imageTexture = new sf::Texture();
		imageTexture->create(image.header.columnCount, image.header.rowCount);

		std::vector<sf::Uint8> rgba8;
		convertImageToRGBA8(image, rgba8);
		imageTexture->update(rgba8.data());

		std::shared_ptr<TextureSprite> textureSprite(new TextureSprite(imageTexture));
		return textureSprite;
	}


	void NetworkRenderer::updateInputImage(const Mnist::ImageSet::Image & image){
		sf::RenderWindow & renderWindow(m_display.getMainWindow().getRenderWindow());
		sf::Vector2u windowSize = renderWindow.getSize();

		std::shared_ptr<TextureSprite> imageSprite = createSpriteFromImage(image);

		imageSprite->setScale(4.f, 4.f);
		imageSprite->setPosition((float)m_renderMargin, windowSize.y/2.f - image.header.rowCount*2.f);
		m_display.getMainWindow().updateImage(imageSprite);
	}

	void NetworkRenderer::updateLabel(int label, bool isCorrect){
		std::string s(Jargon::StringUtilities::format("%d", label));
		std::shared_ptr<sf::Text> labelText(new sf::Text());
		labelText->setColor(isCorrect ? OutlineColor : sf::Color::Red);
		labelText->setString(s);
		labelText->setFont(m_font);
		sf::Vector2u windowSize = m_display.getMainWindow().getRenderWindow().getSize();
		sf::Vector2f position( (windowSize.x - 2*m_renderMargin) * 0.1f, (windowSize.y - 2*m_renderMargin)*0.65f );
		labelText->setPosition(position);
		m_display.getMainWindow().updateLabel(labelText);
	}

	void NetworkRenderer::showBackprop(const Mnist::ImageSet::Image & image, const std::vector<Eigen::MatrixXd> & deltaNablaWeights, const std::vector<Eigen::VectorXd> & deltaNablaBiases){
		HilightingNodeStyler nodeStyler(deltaNablaWeights, deltaNablaBiases);
		WeightedConnectionStyler connectionStyler(m_network.m_layers);
		updateNetworkStructure(nodeStyler, connectionStyler);
		updateInputImage(image);
	}

	void NetworkRenderer::showWeightedImage(const Eigen::VectorXd & weightedImage, size_t width, size_t height, size_t layerIndex, size_t nodeIndex){
		std::vector<sf::Uint8> bitmap;
		convertVectorToRGBA8(weightedImage, width, height, bitmap);

		sf::RenderWindow & renderWindow(m_display.getMainWindow().getRenderWindow());


		sf::Texture * imageTexture = new sf::Texture();
		imageTexture->create((unsigned int)width, (unsigned int)height);
		imageTexture->update(bitmap.data());

		std::shared_ptr<TextureSprite> textureSprite(new TextureSprite(imageTexture));

		size_t displayIndex = nodeIndex;
		for( size_t i = 0; i < layerIndex; i++ ){
			displayIndex += m_nodePositions[i].size();
		}

		sf::Vector2f position(m_nodePositions[layerIndex][nodeIndex]);

		if( m_nodePositions[layerIndex].size() > renderWindow.getSize().y / (height*2) ){
			position.x -= width*1.5f * ( ((nodeIndex+1)%2) + 1.f);
			position.x -= 5.f * (((nodeIndex+1)%2) + 1);
			textureSprite->setPosition(position);

			textureSprite->setScale(1.5f, 1.5f);

			m_display.getMainWindow().setWeightedImageSprite(displayIndex, textureSprite);
		}else{
			position.x -= width*3.f;
			textureSprite->setPosition(position);

			textureSprite->setScale(2.f, 2.f);

			m_display.getMainWindow().setWeightedImageSprite(displayIndex, textureSprite);
		}
	}
}
