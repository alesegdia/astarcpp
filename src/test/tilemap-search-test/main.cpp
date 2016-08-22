
#include <cstdio>
#include <memory>

#include "../common/tilemap-problem.h"
#include "../../astar.h"

int main( int argc, char** argv )
{
	Tilemap tilemap = { 10, 10, {
								  0,1,0,0,0,0,0,0,0,0,
								  0,1,1,1,1,1,1,1,1,0,
								  0,1,0,0,0,0,0,0,0,0,
								  0,1,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,1,1,1,1,1,0,1,1,1,
								  0,1,0,0,0,0,0,1,0,0,
								  0,1,0,1,1,0,1,1,0,0,
								  0,1,0,0,0,0,0,1,0,0,
								  0,1,0,0,0,0,0,0,0,0, } };

	TilemapModel tilemap_model(tilemap);
	AStar<TilemapModel> astar(&tilemap_model);
	TilemapModel::Node::Ptr start_node = std::make_shared<TilemapModel::Node>(0, 0);
	TilemapModel::Node::Ptr target_node = std::make_shared<TilemapModel::Node>(9, 9);

	start_node->G(0);
	start_node->H(tilemap_model.hCost(start_node, target_node));
	start_node->computeF();

	astar.restartSearch(start_node, target_node);

	while( AStarSearchStatus::Running == astar.step() );

	std::cout << astar.processedNodes() << std::endl;
	return 0;
}
