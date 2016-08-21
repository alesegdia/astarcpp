#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * concept<ProblemModel>:
 *	- GetNeighboors( Node::Ptr n );
 */

template <typename NodeType>
class AStarNode
{
public:
	float FCost() const
	{
		return GCost() + HCost();
	}
	float GCost() const
	{
		return m_gCost;
	}
	float HCost() const
	{
		return m_hCost;
	}
	void HCost( float hcost )
	{
		m_hCost = hcost;
	}
	void GCost( float gcost )
	{
		m_gCost = gcost;
	}
	NodeType* Parent() const
	{
		return m_parent;
	}
	void Parent( NodeType* node )
	{
		m_parent = node;
	}

private:
	float m_gCost = 0 ;
	float m_hCost = 0 ;
	NodeType* m_parent;

};

template <typename ProblemModel>
class AStar
{
	typedef typename ProblemModel::Ptr ProblemModelPtr;
	typedef typename ProblemModel::Node::Ptr NodePtr;

	struct NodeCompare
	{
		bool operator()( const NodePtr n1, const NodePtr n2 )
		{
			assert(n1 != nullptr);
			assert(n2 != nullptr);
			n1->FCost() < n2->FCost();
		}
	};

	static bool CompareNodes( NodePtr n1, NodePtr n2 )
	{
		return n1->FCost() > n2->FCost();
	}

public:
	AStar( typename ProblemModel::Ptr problem_model )
		: m_problemModel(problem_model)
	{

	}

	~AStar()
	{
		cleanLists();
	}

	void cleanLists()
	{
		for( auto item : m_openList )
		{
			delete item;
		}

		for( auto item : m_closeList )
		{
			delete item;
		}
	}

	void restartSearch( NodePtr start_node, NodePtr target_node )
	{
		cleanLists();
		m_targetNode = target_node;
		m_openList.push_back(start_node);
	}

	bool step()
	{
		bool ret = false;
		std::sort(m_openList.begin(), m_openList.end(),
				  [](NodePtr n1, NodePtr n2) { return n1->GCost() > n2->GCost(); });
		NodePtr current_node = m_openList.back();
		m_openList.erase(m_openList.begin() + m_openList.size() - 1);
		if( m_problemModel->equal(m_targetNode, current_node) )
		{
			ret = true;
		}
		else
		{
			processNode( current_node );
		}
		m_closeList.push_back( current_node );

		return ret;
	}

	void processNode( NodePtr current_node )
	{
		std::vector<NodePtr> neighboors = m_problemModel->getNeighboors(current_node, m_targetNode);
		for( NodePtr neigh : neighboors )
		{
			auto checker = [&](NodePtr item) {
				return m_problemModel->equal( neigh, item );
			};

			auto found_in_open = std::find_if( m_openList.begin(), m_openList.end(), checker );

			if( found_in_open != m_openList.end() )
			{
				if( ((*found_in_open)->FCost()) > neigh->FCost() )
				{
					delete neigh;
					continue;
				}
			}

			auto found_in_close = std::find_if( m_closeList.begin(), m_closeList.end(), checker );

			if( found_in_close != m_closeList.end() )
			{
				if( ((*found_in_close)->FCost()) > neigh->FCost() )
				{
					delete neigh;
					continue;
				}
			}

			m_closeList.erase( std::remove_if( m_closeList.begin(), m_closeList.end(), checker ), m_closeList.end() );
			m_openList.erase(  std::remove_if( m_openList.begin(),  m_openList.end(),  checker ), m_openList.end() );

			m_openList.push_back(neigh);
		}
	}

private:
	ProblemModelPtr m_problemModel;
	//std::priority_queue<NodePtr, std::vector<NodePtr>, NodeCompare> m_openList;
	std::vector<NodePtr> m_openList;
	std::vector<NodePtr> m_closeList;
	NodePtr m_targetNode;

};
