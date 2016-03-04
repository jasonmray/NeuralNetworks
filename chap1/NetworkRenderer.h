
#ifndef J_NETWORKRENDERER_H
#define J_NETWORKRENDERER_H

#include "Display.h"
#include "Network.h"

#include "SFML/Graphics/Font.hpp"

namespace J{

	class NetworkRenderer{
		public:
			NetworkRenderer(Display & display, J::Network & network);
			~NetworkRenderer();

			void updateNetworkStructure();
			void updateInputImage(const Mnist::ImageSet::Image & image);
			void updateLabel(int label, bool isCorrect);
			void showBackprop(const Mnist::ImageSet::Image & image, const std::vector<Eigen::MatrixXd> & deltaNablaWeights, const std::vector<Eigen::VectorXd> & deltaNablaBiases);
			void showWeightedImage(const Eigen::VectorXd & weightedImage, size_t width, size_t height, size_t layerIndex, size_t nodeIndex);

		private:
			static sf::Color OutlineColor;
			static sf::Color FillColor;
			static sf::Color HilightColor;
			static sf::Color ConnectionColor;
			static float DefaultCircleDrawRadius;
			static unsigned int DefaultRenderMargin;


			class NodeStyler{
				public:
					virtual ~NodeStyler() = default;
					virtual void styleNode(sf::CircleShape & shape, int column, int nodeIndex) = 0;
			};

			class DefaultNodeStyler : public NodeStyler{
				public:
					void styleNode(sf::CircleShape & shape, int column, int nodeIndex);
			};

			class HilightingNodeStyler : public NodeStyler{
				public:
					HilightingNodeStyler(const std::vector<Eigen::MatrixXd> & deltaNablaWeights, const std::vector<Eigen::VectorXd> & deltaNablaBiases);
					void styleNode(sf::CircleShape & shape, int column, int nodeIndex);
				private:
					const std::vector<Eigen::MatrixXd> & m_deltaNablaWeights;
					const std::vector<Eigen::VectorXd> & m_deltaNablaBiases;
			};

			class ConnectionStyler{
				public:
					virtual ~ConnectionStyler() = default;
					virtual void styleConnection(size_t layerIndex, size_t nodeIndex, size_t inputNodeIndex, sf::Vertex & startVertex, sf::Vertex & endVertex) = 0;
			};

			class WeightedConnectionStyler : public ConnectionStyler{
				public:
					WeightedConnectionStyler(const std::vector<NetworkLayer> & layers);
					void styleConnection(size_t layerIndex, size_t nodeIndex, size_t inputNodeIndex, sf::Vertex & startVertex, sf::Vertex & endVertex);
				private:
					std::vector<Eigen::MatrixXd> m_layerWeights;
			};

			void updateNetworkStructure(NodeStyler & nodeStyler, ConnectionStyler & connectionStyler);
			std::shared_ptr<sf::VertexArray> generateConnectionLines(ConnectionStyler & connectionStyler);
			void calculateNodeParameters();


			Display & m_display;
			const J::Network & m_network;

			sf::Font m_font;
			unsigned int m_renderMargin;
			float m_circleDrawRadius;
			std::vector<std::vector<sf::Vector2f> > m_nodePositions;
	};

}

#endif
