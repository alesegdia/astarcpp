
#include <cstdio>

#include "matrix.h"
#include "../../astar.h"

typedef Matrix2D<int> Tilemap;

class TilemapModel
{
public:

	class Node : public AStarNode<Node>
	{
	public:
		typedef Node* Ptr;
		Node( int x, int y ) : m_x(x), m_y(y) { }
		int x() { return m_x; }
		int y() { return m_y; }

	private:
		int m_x, m_y;
	};

	typedef TilemapModel* Ptr;
	typedef Node::Ptr NodePtr;

	TilemapModel( Matrix2D<int> map )
		: m_map(map)
	{

	}

	float distance( NodePtr n1, NodePtr n2 )
	{
		float dx = n1->x() - n2->x();
		float dy = n1->y() - n2->y();
		return sqrtf( dx * dx + dy * dy );
	}

	std::vector<NodePtr> getNeighboors( NodePtr node, NodePtr target_node )
	{
		std::vector<NodePtr> neighboors;

		const int x = node->x();
		const int y = node->y();

		if( x > 0 && m_map.get( x - 1, y ) == 0 )
		{
			neighboors.push_back(new Node( x - 1, y ));
			neighboors.back()->GCost(node->GCost() + 1);
			neighboors.back()->HCost(distance(node, target_node));
			neighboors.back()->Parent(node);
		}

		if( x < m_map.cols() - 1 && m_map.get( x + 1, y ) == 0 )
		{
			neighboors.push_back(new Node( x + 1, y ));
			neighboors.back()->GCost(node->GCost() + 1);
			neighboors.back()->HCost(distance(node, target_node));
			neighboors.back()->Parent(node);
		}

		if( y > 0 && m_map.get( x, y - 1 ) == 0 )
		{
			neighboors.push_back(new Node( x, y - 1 ));
			neighboors.back()->GCost(node->GCost() + 1);
			neighboors.back()->HCost(distance(node, target_node));
			neighboors.back()->Parent(node);
		}

		if( y < m_map.rows() - 1 && m_map.get( x, y + 1 ) == 0 )
		{
			neighboors.push_back(new Node( x, y + 1 ));
			neighboors.back()->GCost(node->GCost() + 1);
			neighboors.back()->HCost(distance(node, target_node));
			neighboors.back()->Parent(node);
		}

		return neighboors;
	}

	bool equal( Node::Ptr n1, Node::Ptr n2 )
	{
		assert(nullptr != n1);
		assert(nullptr != n2);

		return	n1->x() == n2->x() &&
				n1->y() == n2->y();
	}

private:
	Tilemap m_map;

};

int main( int argc, char** argv )
{
	Tilemap tilemap = { 10, 10, {
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0,
								  0,0,0,0,0,0,0,0,0,0, } };
	TilemapModel tilemap_model(tilemap);
	AStar<TilemapModel> astar(&tilemap_model);
	TilemapModel::Node::Ptr start_node = new TilemapModel::Node(0, 0);
	TilemapModel::Node::Ptr target_node = new TilemapModel::Node(5, 5);
	astar.restartSearch(start_node, target_node);

	while( false == astar.step() );

	delete start_node;
	delete target_node;
	printf("exit\n");
	return 0;
}
