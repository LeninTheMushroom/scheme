#include "circuitbuilder.h"

void CircuitBuilder::clear()
{
    command_list.clear();
    node_list.clear();
    node_info.clear();
    edge_list.clear();
    redrawn_edges.clear();
    delete ogdf_circuit;
    ogdf_circuit = new GraphProcessor();
}

void CircuitBuilder::build_circuit(QString data,
                   unsigned short algorithm_runs,
                   unsigned short sparcity )
{
    build_command_matrix(data);
    ogdf_circuit->set_orthogonal_layout(algorithm_runs, sparcity);
    optimize_layout();
    draw_nodes();
    draw_edges();
}

void CircuitBuilder::build_command_matrix(QString data)
{
    QMessageBox messageBox;
    messageBox.setFixedSize(500,200);
    switch (parser->parse_command_string(data, command_list)) {
    case 1:
        process_matrix_to_graph();
        break;
    case -1:
        messageBox.critical(0,"Ошибка","Подана пустая строка");
        break;
    case -2:
        messageBox.critical(0,"Ошибка","Строка команд не заканчивается точкой");
        break;
    case -3:
        messageBox.critical(0,"Ошибка","Неправильный формат команд");
        break;
    default:
        messageBox.critical(0,"Ошибка","Текст введен неправильно!");
        break;
    }
}

void CircuitBuilder::process_matrix_to_graph()
{
    QVector <int> joint_list;
    for(int it=0; it<command_list.size(); it++)
    {
        //amplifier should not amplify nothing (or unconnected otherwise nodes)
        //possible error
        if(!joint_list.contains(command_list.at(it)->in_2))
            joint_list.push_back(command_list.at(it)->in_2);
        if(!joint_list.contains(command_list.at(it)->out))
            joint_list.push_back(command_list.at(it)->out);
        add_node(command_list.at(it)->type, command_list.at(it)->number);
    }
    for(int it=0; it<joint_list.size(); it++)
    {
        add_node(' ', it+1);
    }
    // vector of nodes should then be as follows
    // [ node1, node2 ... node(command_list.size), joint1, joint2 ... joint(joint_list.size), ]
    // [   0       1                c                c+1     c+2                c+j           ]
    for(int i=0; i<command_list.size(); i++)
    {
        if(command_list.at(i)->type == 'A' || command_list.at(i)->type == 'a')
            add_edge(command_list.size() + command_list.at(i)->in_1 - 1, i);

        add_edge(command_list.size() + command_list.at(i)->in_2 - 1, i);
        add_edge(i, command_list.size() + command_list.at(i)->out - 1);
    }
}

void CircuitBuilder::add_edge(int from, int to)
{
    edge_list.push_back(ogdf_circuit->add_edge(node_list.at(from), node_list.at(to)));
}

void CircuitBuilder::add_node(QChar type, int num, QColor color)
{
    QSize image_size(40, 40);
    if(type == ' ')
    {
        image_size.setHeight(10);
        image_size.setWidth(10);
    }
    node_list.push_back(ogdf_circuit->add_node(image_size));
    node_info.push_back(new NodeInfo(type, num, image_size, color));
}

int CircuitBuilder::get_connected_edge(bool incoming, int node_vector_position)
{
    //not for amplifiers
    int edge_vector_position = 0;
    for(int i=0; i<node_vector_position; i++)
    {
        if(command_list.at(i)->type == 'A' || command_list.at(i)->type == 'a')
            edge_vector_position+=3;
        else
            edge_vector_position+=2;
    }
    if(incoming)
        return edge_vector_position;
    else
        return edge_vector_position+1;
}

void CircuitBuilder::optimize_layout()
{
    for(int i=0; i<command_list.size(); i++)
        optimize_node_rotation(i);

    for(int i=0; i<command_list.size(); i++)
    {
        QPointF last_inbound_point = ogdf_circuit->return_edge_points(edge_list[get_connected_edge(1,i)]).last();
        QVector<QPointF> points;
        points = ogdf_circuit->return_edge_points
                (edge_list[get_connected_edge(0,i)]);

        int node_x = ogdf_circuit->get_coordinates(node_list[i])->x;
        int node_y = ogdf_circuit->get_coordinates(node_list[i])->y;
        if( (node_info[i]->type!='A' && node_info[i]->type!='a') &&
                (last_inbound_point.x()!=points.first().x() && last_inbound_point.y() != points.first().y()))
        {
            redrawn_edges.push_back(new Coordinates(last_inbound_point.x(),
                                                    last_inbound_point.y(),
                                                    node_x,
                                                    node_y));
            int first_x = points.first().x();
            int first_y = points.first().y();
            int last_x = points.at(1).x();
            int last_y = points.at(1).y();
            int dx = first_x - last_x;
            int dy = first_y - last_y;

            if(dx == 0)
            {
                int additive = 0;
                if(dy>0)
                    additive = -node_info.at(i)->size.height();
                else
                    additive = +node_info.at(i)->size.height();

                ogdf_circuit->fix_node_coordinates(node_list[i],
                                                   node_x,
                                                   node_y-dy/2+additive/2);
                redrawn_edges.push_back(new Coordinates(node_x,node_y,
                                                        node_x,node_y-dy/2));
                redrawn_edges.push_back(new Coordinates(node_x,node_y-dy/2+additive,
                                                        last_x,last_y));
            }
            else
            {
                int additive = 0;
                if(dx>0)
                    additive = -node_info.at(i)->size.width();
                else
                    additive = +node_info.at(i)->size.width();

                ogdf_circuit->fix_node_coordinates(node_list[i],
                                                   node_x-dx/2+additive/2,
                                                   node_y);
                redrawn_edges.push_back(new Coordinates(node_x,node_y,
                                                        node_x-dx/2,node_y));
                redrawn_edges.push_back(new Coordinates(node_x-dx/2+additive,node_y,
                                                        last_x,last_y));
            }
            for(int it=1; it<points.size()-1; it++)
            {
                redrawn_edges.push_back(new Coordinates(points.at(it).x(),points.at(it).y(),
                                                        points.at(it+1).x(),points.at(it+1).y()));
            }
            ogdf_circuit->delete_edge(edge_list[get_connected_edge(0,i)]);
            edge_list[get_connected_edge(0,i)] = NULL;
        }
    }
}

void CircuitBuilder::optimize_node_rotation(unsigned node_pos)
{
    QVector<QPointF> points = ogdf_circuit->return_edge_points
            (edge_list[get_connected_edge(0,node_pos)]);
    int dx = points.at(1).x() - points.first().x();
    int dy = points.at(1).y() - points.first().y();
    //default rotation is right
    if(dx<0)//left
        node_info.at(node_pos)->rotation = 180;
    else
    {
        if(dy>0)//down
            node_info.at(node_pos)->rotation = 90;
        else
        {
            if(dy<0)//up
                node_info.at(node_pos)->rotation = 270;
        }
    }
}

void CircuitBuilder::draw_nodes()
{
    for (int it=0; it<node_list.size(); it++)
    {
        if(node_list[it] != NULL)
        {
            Coordinates *c = ogdf_circuit->get_coordinates(node_list[it]);
            scene->draw_node(c, *node_info[it]);
        }
    }
}

void CircuitBuilder::draw_edges()
{
    for (int it=0; it<edge_list.size(); it++)
    {
        if(edge_list[it] != NULL)
        {
            QVector<QPointF> points = ogdf_circuit->return_edge_points(edge_list[it]);
            scene->draw_edge(points);
        }
    }
    for(int i=0; i<redrawn_edges.size(); i++)
    {
        scene->draw_edge(redrawn_edges.at(i));
    }
}
