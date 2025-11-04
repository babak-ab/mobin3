#ifndef RETICLE_H
#define RETICLE_H

#include <QSize>
#include <QPoint>
#include <QWidget>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QQuickPaintedItem>

#include <iostream>

class Reticle : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(quint8 color READ color WRITE setReticleColor NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 shape READ shape WRITE setReticleShape NOTIFY sigDataChanged)
    Q_PROPERTY(int reticlePositionX READ reticlePositionX WRITE setReticlePositionX NOTIFY sigDataChanged)
    Q_PROPERTY(int reticlePositionY READ reticlePositionY WRITE setReticlePositionY NOTIFY sigDataChanged)

public:
    explicit Reticle(QQuickPaintedItem* parent = Q_NULLPTR);

public:
    Q_INVOKABLE void setReticleColor(quint8 color);
    Q_INVOKABLE void setReticlePositionX(int x);
    Q_INVOKABLE void setReticlePositionY(int y);
    Q_INVOKABLE void setReticleShape(quint8 shape);

    quint8 color() const;
    quint8 shape() const;
    int reticlePositionX() const;
    int reticlePositionY() const;

private:
    enum ReticleShape
    {
        Cross,
        HollowCross,
        PointCross,
        RectCross,
    };

    QColor m_reticleColor;
    ReticleShape m_reticleShape;
    QPoint m_reticlePosition;
    QColor m_backgroundColor;
    quint8 m_thickness;
    quint16 m_xOffset;
    quint16 m_yOffset;

    void paint(QPainter* painter);
    void drawReticle(QPainter *painter);

Q_SIGNALS:
    void sigDataChanged();

};

#endif // RETICLE_H
