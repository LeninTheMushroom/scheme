#ifndef DRAWINGSCENE_H
#define DRAWINGSCENE_H

#include "graphprocessor.h"
#include <QGraphicsScene>
#include <QGraphicsSvgItem>

#include <QRectF>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QLineF>
#include <QPolygonF>

struct NodeInfo
{
    int number;
    QChar type;
    QColor color;
    QSize size;
    qreal rotation;
    NodeInfo(QChar t, int n, QSize s, QColor q)
    {
        type=t; number=n; size=s; color=q;
        rotation = 0;
    }
};

class DrawingScene
{
public:
    DrawingScene(QGraphicsScene *scene);
    ~DrawingScene(){scene->clear();}

    void draw_node(Coordinates *c, NodeInfo info);
    void draw_edge(QVector<QPointF> points);
    void draw_edge(Coordinates *c);
    void draw_arrow(const QPointF& start, const QPointF& end, const QColor& color);
    void set_element_image_dimensions(int x, int y);
    void set_graphics_pack(QString path);
    int image_x();
    int image_y();
private:
    QGraphicsScene *scene;
    QString graphics_pack;
};

#endif // DRAWINGSCENE_H
