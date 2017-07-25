#ifndef GRAPHPROCESSOR_H
#define GRAPHPROCESSOR_H

#include <QSize>
#include <QPoint>
#include <QVector>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/fileformats/GraphIO.h>


struct Coordinates
{
    double x, y, h, w;
    Coordinates(double x, double y, double h, double w)
    {
        this->x = x; this->y = y; this->w = w; this->h = h;
    }
    Coordinates(){}
};

class GraphProcessor
{
public:
    GraphProcessor()
    {
        G = new ogdf::Graph();
        GA = new ogdf::GraphAttributes(*G,
                                       ogdf::GraphAttributes::nodeGraphics |
                                       ogdf::GraphAttributes::edgeGraphics |
                                       ogdf::GraphAttributes::edgeArrow |
                                       ogdf::GraphAttributes::nodeLabel |
                                       ogdf::GraphAttributes::edgeLabel |
                                       ogdf::GraphAttributes::edgeStyle |
                                       ogdf::GraphAttributes::nodeStyle |
                                       ogdf::GraphAttributes::nodeTemplate);
    }
    ~GraphProcessor()
    {
        G->clear();
        delete GA;
        delete G;
    }
    void delete_edge(ogdf::edge &e);
    void fix_node_coordinates(ogdf::node &target, int x, int y);
    void set_orthogonal_layout(unsigned short algorithm_runs, unsigned short sparcity);
    ogdf::edge add_edge(const ogdf::node &source,
                   const ogdf::node &target);
    ogdf::node add_node(const QSize &size);
    QVector<QPointF> return_edge_points(ogdf::edge &e);
    Coordinates* get_coordinates(ogdf::node &source);
private:
    ogdf::Graph *G;
    ogdf::GraphAttributes *GA;
};

#endif // GRAPHPROCESSOR_H
