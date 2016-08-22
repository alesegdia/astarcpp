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
	float F() const
	{
		return m_fCost;
	}
	float G() const
	{
		return m_gCost;
	}
	float H() const
	{
		return m_hCost;
	}
	void H( float hcost )
	{
		m_hCost = hcost;
	}
	void G( float gcost )
	{
		m_gCost = gcost;
	}

	void computeF()
	{
		m_fCost = m_gCost + m_hCost;
	}

private:
	float m_gCost = 0 ;
	float m_hCost = 0 ;
	float m_fCost = 0 ;

};

enum class AStarSearchStatus {
	Uninitialised,
	Finished,
	Running,
};

template <typename ProblemModel>
class AStar
{
	typedef typename ProblemModel::Ptr ProblemModelPtr;
	typedef typename ProblemModel::NodePtr NodePtr;

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

	void restartSearch( NodePtr start_node, NodePtr target_node )
	{
		m_closeList.clear();
		m_openList.clear();
		m_targetNode = target_node;
		m_openList.push_back(start_node);
		m_processedNodes = 0;
		m_currentNode = start_node;
	}

	void computeSolution()
	{
		NodePtr n = m_targetNode;
		while( nullptr != n )
		{
			m_solution.insert(m_solution.begin(), n);
			n = n->Parent();
		}
	}

	const std::vector<NodePtr>& solution() const
	{
		return m_solution;
	}

	AStarSearchStatus step()
	{
		// stats
		m_processedNodes++;

		m_currentNode = popBestNode();
		if( m_problemModel->equal(m_targetNode, m_currentNode) )
		{
			m_searchStatus = AStarSearchStatus::Finished;
			m_targetNode->Parent(m_currentNode);
			computeSolution();
		}
		else
		{
			m_searchStatus = AStarSearchStatus::Running;
			processNode( m_currentNode );
		}
		m_closeList.push_back( m_currentNode );

		return m_searchStatus;
	}

	size_t processedNodes()
	{
		return m_processedNodes;
	}

	const std::vector<NodePtr>& openList() const
	{
		return m_openList;
	}

	const std::vector<NodePtr>& closeList() const
	{
		return m_closeList;
	}

	const NodePtr& currentNode() const
	{
		return m_currentNode;
	}

	AStarSearchStatus status()
	{
		return m_searchStatus;
	}

private:

	NodePtr popBestNode()
	{
		sortByCost( m_openList );
		NodePtr best = m_openList.back();
		m_openList.erase(m_openList.begin() + m_openList.size() - 1);
		return best;
	}

	void sortByCost( std::vector<NodePtr>& v )
	{
		std::sort(v.begin(), v.end(), [](NodePtr n1, NodePtr n2) { return n1->F() > n2->F(); });
	}

	void processNode( NodePtr current_node )
	{
		std::vector<NodePtr> neighboors = m_problemModel->getNeighboors(current_node, m_targetNode);
		for( NodePtr neigh : neighboors )
		{
			neigh->G( current_node->G() + m_problemModel->gCost( current_node, neigh ) );

			if( hasBetterEqualNode( m_openList, neigh ) || hasBetterEqualNode( m_closeList, neigh ) )
			{
				continue;
			}

			removeDuplicates( m_closeList, neigh );
			removeDuplicates( m_openList, neigh );

			neigh->H( m_problemModel->hCost( neigh, m_targetNode ) );
			neigh->computeF();
			neigh->Parent(current_node);
			m_openList.push_back(neigh);
		}
	}

	bool hasBetterEqualNode( std::vector<NodePtr>& v, NodePtr node )
	{
		auto found = std::find_if( v.begin(), v.end(), [&](NodePtr item) { return m_problemModel->equal( node, item ); } );
		return found != v.end() && ((*found)->G()) < node->G();
	}

	void removeDuplicates( std::vector<NodePtr>& v, NodePtr node )
	{
		v.erase( std::remove_if( v.begin(), v.end(), [&](NodePtr item) { return m_problemModel->equal( node, item ); } ), v.end() );
	}

	ProblemModelPtr m_problemModel;
	std::vector<NodePtr> m_openList;
	std::vector<NodePtr> m_closeList;
	NodePtr m_targetNode;
	size_t m_processedNodes = 0;
	NodePtr m_currentNode;
	AStarSearchStatus m_searchStatus = AStarSearchStatus::Uninitialised;
	std::vector<NodePtr> m_solution;

};
