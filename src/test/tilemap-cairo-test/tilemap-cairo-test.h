#pragma once

#include <memory>

#include <SDL2/SDL.h>
#include <cairo.h>

#include "../common/tilemap-problem.h"

class AStarTilemapCairoTest
{
public:

	AStarTilemapCairoTest( cairo_t* pctx )
		: ctx(pctx)
	{
		m_tilemap.reset(new Tilemap(10, 10));
		m_tilemap->data( {
							 0,1,0,0,0,0,0,0,0,0,
							 0,1,1,1,1,1,1,1,1,0,
							 0,1,0,0,0,0,0,0,0,0,
							 0,1,0,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,0,
							 0,1,1,1,1,1,0,1,1,1,
							 0,1,0,0,0,0,0,1,0,0,
							 0,1,0,1,1,0,1,1,0,0,
							 0,1,0,0,0,0,0,1,0,0,
							 0,1,0,0,0,0,0,0,0,0, } );
		m_tilemapModel.reset( new TilemapModel(*m_tilemap) );
		m_startNode = std::make_shared<TilemapModel::Node>(0, 0);
		m_targetNode = std::make_shared<TilemapModel::Node>(9, 9);
		m_startNode->G(0);
		m_startNode->H(m_tilemapModel->hCost(m_startNode, m_targetNode));
		m_startNode->computeF();
		m_astar.reset( new AStar<TilemapModel>(m_tilemapModel.get()) );
		m_astar->restartSearch(m_startNode, m_targetNode);

	}

	void KeyDown( SDL_Scancode key )
	{
		if( key == SDL_SCANCODE_SPACE )
		{
			if( AStarSearchStatus::Finished != m_astar->status() )
			{
				m_astar->step();
			}
		}
	}

	void Step()
	{
		cairo_set_source_rgb (ctx, 1, 1, 1);
		cairo_paint(ctx);
		drawNodes( m_astar->openList(), 0, 1, 0 );
		drawNodes( m_astar->closeList(), 1, 0, 0 );

		cairo_set_source_rgb(ctx, 0, 0, 0);
		for( int i = 0; i < m_tilemap->rows(); i++ )
		{
			for( int j = 0; j < m_tilemap->cols(); j++ )
			{
				if( m_tilemap->get(j, i) == 1 )
				{
					drawTile(j, i);
				}
			}
		}

		cairo_set_source_rgb(ctx, 0, 0, 1);
		drawNode( m_astar->currentNode(), 0, 0, 1 );
		drawNode( m_targetNode, 0, 1, 1 );

		cairo_set_source_rgb(ctx, 0, 0, 0);

		for( int i = 0; i < 10; i++ )
		{
			for( int j = 0; j < 10; j++ )
			{
				cairo_set_source_rgb(ctx, 0, 0, 0);
				cairo_rectangle(ctx, i * SCALE + SCALE / 4, j * SCALE + SCALE / 4, SCALE / 2, SCALE / 2 );
			}
		}
		drawNodes( m_astar->solution(), 0.45, 0.78, 1 );

	}

	void drawNodes( const std::vector<TilemapModel::NodePtr>& nodes, double r, double g, double b )
	{
		for( auto node : nodes )
		{
			drawNode(node, r, g, b);
		}
	}

	void drawNode( TilemapModel::NodePtr node, double r, double g, double b )
	{
		cairo_set_source_rgb(ctx, r, g, b);
		int x = node->x();
		int y = node->y();
		drawTile( x, y );
		cairo_set_source_rgb(ctx, 1 - r, 1 - g, 1 - b );
		char buffer[64];
		cairo_move_to(ctx, x * SCALE + 1, y * SCALE + 15);
		sprintf(buffer, "G=%.2f", node->G());
		cairo_show_text(ctx, buffer);

		cairo_move_to(ctx, x * SCALE + 1, y * SCALE + 30);
		sprintf(buffer, "H=%.2f", node->H());
		cairo_show_text(ctx, buffer);

		cairo_move_to(ctx, x * SCALE + 1, y * SCALE + 45);
		sprintf(buffer, "F=%.2f", node->F());
		cairo_show_text(ctx, buffer);


	}

	void drawTile( int x, int y )
	{
		cairo_rectangle(ctx, x * SCALE, y * SCALE, SCALE, SCALE);
		cairo_fill(ctx);
	}

private:
	const int SCALE = 50;

	cairo_t* ctx;
	std::shared_ptr<TilemapModel> m_tilemapModel;
	std::shared_ptr<Tilemap> m_tilemap;
	std::shared_ptr<AStar<TilemapModel>> m_astar;
	TilemapModel::Node::Ptr m_startNode;
	TilemapModel::Node::Ptr m_targetNode;

};
