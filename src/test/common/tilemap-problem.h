#pragma once

#include "../../astar.h"
#include "matrix.h"

typedef Matrix2D<int> Tilemap;

class TilemapModel
{
public:

	class Node : public AStarNode<Node>
	{
	public:
		typedef std::shared_ptr<Node> Ptr;
		Node( int x, int y ) : m_x(x), m_y(y) { }
		int x() { return m_x; }
		int y() { return m_y; }

		Ptr Parent() const
		{
			return m_parent;
		}
		void Parent( Ptr node )
		{
			m_parent = node;
		}

	private:
		Node::Ptr m_parent;
		int m_x, m_y;

	};

	typedef TilemapModel* Ptr;
	typedef Node::Ptr NodePtr;

	TilemapModel( Matrix2D<int> map )
		: m_map(map)
	{

	}

	float hCost( NodePtr src, NodePtr target )
	{
		return distance( src, target );
	}

	float gCost( NodePtr src, NodePtr target )
	{
		return 1;
	}

	std::vector<NodePtr> getNeighboors( NodePtr node, NodePtr target_node )
	{
		std::vector<NodePtr> neighboors;

		const int x = node->x();
		const int y = node->y();

		if( x > 0 && m_map.get( x - 1, y ) == 0 )
		{
			neighboors.push_back(std::make_shared<Node>( x - 1, y ));
		}

		if( x < m_map.cols() - 1 && m_map.get( x + 1, y ) == 0 )
		{
			neighboors.push_back(std::make_shared<Node>( x + 1, y ));
		}

		if( y > 0 && m_map.get( x, y - 1 ) == 0 )
		{
			neighboors.push_back(std::make_shared<Node>( x, y - 1 ));
		}

		if( y < m_map.rows() - 1 && m_map.get( x, y + 1 ) == 0 )
		{
			neighboors.push_back(std::make_shared<Node>( x, y + 1 ));
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
	float manhattanDistance( NodePtr n1, NodePtr n2 )
	{
		int dx = n1->x() - n2->x();
		int dy = n1->y() - n2->y();
		int dist = abs(dx) + abs(dy);
		return dist;
	}

	float euclideanDistance( NodePtr n1, NodePtr n2 )
	{
		float dx = float(n1->x()) - float(n2->x());
		float dy = float(n1->y()) - float(n2->y());
		return sqrtf(dx * dx + dy * dy);
	}

	float distance( NodePtr n1, NodePtr n2 )
	{
		//return euclideanDistance(n1, n2);
		return manhattanDistance(n1, n2);
	}

	Tilemap m_map;

};

