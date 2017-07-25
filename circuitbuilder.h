#ifndef CIRCUITBUILDER_H
#define CIRCUITBUILDER_H

#include <QVector>
#include <QColor>
#include <QMessageBox>
#include "parser.h"
#include "drawingscene.h"


class CircuitBuilder
{
private:
    GraphProcessor *ogdf_circuit;
    DrawingScene *scene;
    Parser *parser;
    QVector<Command*> command_list;

    QVector<Coordinates*> redrawn_edges;
    QVector<ogdf::node> node_list;
    QVector<NodeInfo*> node_info;
    QVector<ogdf::edge> edge_list;

public:
    CircuitBuilder(QGraphicsScene *scene)
    {
        this->ogdf_circuit = new GraphProcessor();
        this->scene = new DrawingScene(scene);
        this->parser = new Parser();
    }
    void change_graphics_pack(QString path)
    {
        this->scene->set_graphics_pack(path);
    }

    void clear();
    void build_circuit(QString data,
                       unsigned short algorithm_runs,
                       unsigned short sparcity );
    void build_command_matrix(QString data);
    void process_matrix_to_graph();
    void add_edge(int from, int to);
    void add_node(QChar type, int num, QColor color = QColor("black"));
    int get_connected_edge(bool incoming, int node_vector_position);
    void optimize_layout();
    void optimize_node_rotation(unsigned node_pos);
    void draw_nodes();
    void draw_edges();
};

#endif // CIRCUITBUILDER_H
