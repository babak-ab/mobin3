#ifndef RETICLE_H
#define RETICLE_H

#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QWidget>

class Reticle : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(quint8 color READ color NOTIFY sigDataChanged)


public:
    explicit Reticle(QQuickPaintedItem* parent = Q_NULLPTR);

public:
    void paint(QPainter* painter);

    Q_INVOKABLE void setReticleColor(quint8 color);
    Q_INVOKABLE void setReticleSize(quint8 thickness);
    Q_INVOKABLE void moveReticlePosition(quint16 xOffset, quint16 yOffset);

    quint8 color() const;

private:
    QColor m_reticleColor;
    QColor m_backgroundColor;
    quint8 m_thickness;
    quint16 m_xOffset;
    quint16 m_yOffset;

Q_SIGNALS:
    void sigDataChanged();

};

#endif // RETICLE_H
