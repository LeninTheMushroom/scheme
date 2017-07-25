#include "drawingscene.h"

DrawingScene::DrawingScene(QGraphicsScene *scene)
{
    this->scene = scene;
    graphics_pack = "graphics1/";
}

void DrawingScene::set_graphics_pack(QString path)
{
    this->graphics_pack = path + "/";
}

void DrawingScene::draw_node(Coordinates *c, NodeInfo info)
{
    QGraphicsSvgItem *item;
    switch(info.type.toUpper().toLatin1())
    {
    case 32: // ' '
        item = new QGraphicsSvgItem(graphics_pack+"joint.svg");
        break;
    case 65: // 'A'
        item = new QGraphicsSvgItem(graphics_pack+"A.svg");
        break;
    case 67: // 'C'
        item = new QGraphicsSvgItem(graphics_pack+"C.svg");
        break;
    case 73: // 'I'
        item = new QGraphicsSvgItem(graphics_pack+"I.svg");
        break;
    case 75: // 'K'
        item = new QGraphicsSvgItem(graphics_pack+"K.svg");
        break;
    case 76: // 'L'
        item = new QGraphicsSvgItem(graphics_pack+"L.svg");
        break;
    case 82: // 'R'
        item = new QGraphicsSvgItem(graphics_pack+"R.svg");
        break;
    case 85: // 'U'
        item = new QGraphicsSvgItem(graphics_pack+"U.svg");
        break;
    default:
        item = new QGraphicsSvgItem(graphics_pack+"joint.svg");
        break;
    }
    item->setPos(c->x - info.size.width()/2, c->y - info.size.height()/2);
    item->setTransformOriginPoint(info.size.width()/2,info.size.height()/2);
    item->setRotation(info.rotation);
    item->resetTransform();
    scene->addItem(item);

    QString name;
    if(info.type != ' ')
        name.append(info.type.toUpper());
    name.append(QString::number(info.number));
    QGraphicsTextItem *text = new QGraphicsTextItem(name);
    scene->addItem(text);
    int dy = info.size.height();
    if(dy < 26)
        dy = 26;
    int dx = info.size.width()/4;
    if(dx < 8)
        dx = 8;
    text->setPos(c->x + dx, c->y - dy);
    text->setScale(1.2);
}

void DrawingScene::draw_edge(Coordinates *c)
{
    scene->addLine(c->x, c->y, c->h, c->w);
    //scene->addLine(c->x,c->y,c->w,c->h);
}

void DrawingScene::draw_edge(QVector<QPointF> points)
{
    QPainterPath path(points.first());
    for(int i=1; i<points.size(); i++)
    {
        path.lineTo(points[i]);
    }
    scene->addPath(path, QPen(QColor("black")));
    draw_arrow(points.at(points.size()-2),
               points.at(points.size()-1),
               QColor("black"));
}

void DrawingScene::draw_arrow(const QPointF &start, const QPointF &end, const QColor &color)
{
    qreal Pi = 3.14;
    qreal arrowSize = 10;

    QPolygonF arrowHead;

    QLineF line(end, start);

    double angle = ::acos(line.dx() / line.length());

    if ( line.dy() >= 0 )
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line.p1() + QPointF(sin(angle+Pi/3)*arrowSize,
                                          cos(angle+Pi/3)*arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle+Pi-Pi/3)*arrowSize,
                                          cos(angle+Pi-Pi/3)*arrowSize);


    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;

    scene->addPolygon(arrowHead, QPen(color), QBrush(color));

    scene->addLine(line, QPen(color));
}
