#include "graphprocessor.h"

void GraphProcessor::set_orthogonal_layout(unsigned short algorithm_runs, unsigned short sparcity)
{
    ogdf::PlanarizationLayout pl;

    ogdf::SubgraphPlanarizer *crossMin = new ogdf::SubgraphPlanarizer;


    ogdf::FastPlanarSubgraph *ps = new ogdf::FastPlanarSubgraph;

    ps->runs(algorithm_runs);
    ogdf::VariableEmbeddingInserter *ves = new ogdf::VariableEmbeddingInserter;
    ves->removeReinsert(ogdf::rrAll);

    crossMin->setSubgraph(ps);
    crossMin->setInserter(ves);
    pl.setCrossMin(crossMin);

    ogdf::EmbedderMinDepthMaxFaceLayers *emb = new ogdf::EmbedderMinDepthMaxFaceLayers;
    pl.setEmbedder(emb);

    ogdf::OrthoLayout *ol = new ogdf::OrthoLayout;
    ol->separation(sparcity);
    ol->cOverhang(0.4);
    pl.setPlanarLayouter(ol);
    pl.call(*GA);

    ogdf::GraphIO::writeGML(*GA, "graph_scheme.gml");
}

void GraphProcessor::delete_edge(ogdf::edge &e)
{
    G->delEdge(e);
}

void GraphProcessor::fix_node_coordinates(ogdf::node &target, int x, int y)
{
    GA->x(target) = x;
    GA->y(target) = y;
}

ogdf::edge GraphProcessor::add_edge(const ogdf::node &source,
                               const ogdf::node &target)
{
    ogdf::edge new_edge = G->newEdge(source, target);
    return new_edge;
}

ogdf::node GraphProcessor::add_node(const QSize &size)
{
    ogdf::node new_node = G->newNode();
    GA->width(new_node) = size.width();
    GA->height(new_node) = size.height();
    return new_node;
}

Coordinates* GraphProcessor::get_coordinates(ogdf::node &source)
{
    Coordinates *c = new Coordinates();
    c->h = GA->height(source);
    c->w = GA->width(source);
    c->x = GA->x(source);
    c->y = GA->y(source);
    return c;
}

QVector<QPointF> GraphProcessor::return_edge_points(ogdf::edge &e)
{
    QVector<QPointF> points_list;
    ogdf::DPolyline& points = GA->bends(e);
    ogdf::List<ogdf::DPoint>::const_iterator iterator = points.begin();
    while(iterator != points.end())
    {
        points_list.push_back(QPointF((*iterator).m_x,(*iterator).m_y));
        iterator++;
    }
    return points_list;
}
