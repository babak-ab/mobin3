#include "reticle.h"

Reticle::Reticle(QQuickPaintedItem* parent)
    : QQuickPaintedItem(parent)
{

    //setFlag(QQuickItem::ItemHasContents);

    m_reticleColor = QColor(66, 173, 245);
    m_reticleShape = Cross;
    m_reticlePosition = QPoint(0, 0);
    m_backgroundColor = Qt::black;
    m_thickness = 4;
    m_xOffset = 0;
    m_yOffset = 0;
}

void Reticle::setReticleColor(quint8 color)
{
    switch (color)
    {
    case 0:
    {
        m_reticleColor = QColor(66, 173, 245);

        break;
    }
    case 1:
    {
        m_reticleColor = Qt::red;

        break;
    }
    case 2:
    {
        m_reticleColor = Qt::green;

        break;
    }
    }

    Q_EMIT sigDataChanged();

}

void Reticle::setReticlePositionX(int x)
{
    m_reticlePosition.setX(x);

    Q_EMIT sigDataChanged();
}

void Reticle::setReticlePositionY(int y)
{
    m_reticlePosition.setY(y);

    Q_EMIT sigDataChanged();
}

void Reticle::setReticleShape(quint8 shape)
{
    ReticleShape newShape =
            static_cast<ReticleShape>(shape);

    m_reticleShape = newShape;

    Q_EMIT sigDataChanged();
}

quint8 Reticle::color() const
{
    if (m_reticleColor == QColor(66, 173, 245)) {
        return 0;
    } else if (m_reticleColor == Qt::red) {
        return 1;
    } else if (m_reticleColor == Qt::green) {
        return 2;
    } else {
        return 0;
    }
}

quint8 Reticle::shape() const
{
    return static_cast<uint8_t>(
                m_reticleShape);
}

int Reticle::reticlePositionX() const
{
    return m_reticlePosition.x();
}

int Reticle::reticlePositionY() const
{
    return m_reticlePosition.y();
}

void Reticle::paint(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    drawReticle(painter);
}

void Reticle::drawReticle(QPainter *painter)
{
    painter->setPen(QPen(QBrush(m_reticleColor),
                         m_thickness / 2, Qt::SolidLine));

    const QSize reticleSize(240, 135);

    const QPoint centerPoint(width() / 2.0, height() / 2.0);

    const QPoint reticleCenterPoint(
                centerPoint.x() + (-1 * m_reticlePosition.x()),
                centerPoint.y() + (-1 * m_reticlePosition.y()));

    QPainterPath path;

    switch (m_reticleShape)
    {
    case Cross:
    {

        path.moveTo(reticleCenterPoint.x() -
                    (reticleSize.width() / 2),
                    reticleCenterPoint.y());

        path.lineTo(reticleCenterPoint.x() +
                    (reticleSize.width() / 2),
                    reticleCenterPoint.y());

        path.moveTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() -
                    (reticleSize.height() / 2));

        path.lineTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() +
                    (reticleSize.height() / 2));

        break;
    }
    case HollowCross:
    {
        const QSize lineSize(
                    2 * reticleSize.width() / 2 / 3,
                    2 * reticleSize.height() / 2 / 3);

        path.moveTo(reticleCenterPoint.x() -
                    (reticleSize.width() / 2),
                    reticleCenterPoint.y());

        path.lineTo(reticleCenterPoint.x() -
                    (reticleSize.width() / 2) +
                    lineSize.width(),
                    reticleCenterPoint.y());

        path.moveTo(reticleCenterPoint.x() +
                    (reticleSize.width() / 2),
                    reticleCenterPoint.y());

        path.lineTo(reticleCenterPoint.x() +
                    (reticleSize.width() / 2) -
                    lineSize.width(),
                    reticleCenterPoint.y());

        path.moveTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() -
                    (reticleSize.height() / 2));

        path.lineTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() -
                    (reticleSize.height() / 2) +
                    lineSize.height());

        path.moveTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() +
                    (reticleSize.height() / 2));

        path.lineTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() +
                    (reticleSize.height() / 2) -
                    lineSize.height());

        break;
    }
    case PointCross:
    {
        const QSize lineSize(
                    2 * reticleSize.width() / 2 / 3,
                    2 * reticleSize.height() / 2 / 3);

        path.moveTo(reticleCenterPoint.x() -
                    (reticleSize.width() / 2),
                    reticleCenterPoint.y());

        path.lineTo(reticleCenterPoint.x() -
                    (reticleSize.width() / 2) +
                    lineSize.width(),
                    reticleCenterPoint.y());

        path.moveTo(reticleCenterPoint.x() +
                    (reticleSize.width() / 2),
                    reticleCenterPoint.y());

        path.lineTo(reticleCenterPoint.x() +
                    (reticleSize.width() / 2) -
                    lineSize.width(),
                    reticleCenterPoint.y());

        path.moveTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() -
                    (reticleSize.height() / 2));

        path.lineTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() -
                    (reticleSize.height() / 2) +
                    lineSize.height());

        path.moveTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() +
                    (reticleSize.height() / 2));

        path.lineTo(reticleCenterPoint.x(),
                    reticleCenterPoint.y() +
                    (reticleSize.height() / 2) -
                    lineSize.height());

        path.addEllipse(reticleCenterPoint,
                        m_thickness / 4,
                        m_thickness / 4);

        break;
    }
    case RectCross:
    {
        const QSize lineSize(
                    2 * reticleSize.width() / 3,
                    2 * reticleSize.height() / 3);

        const QRect reticleRect(
                    reticleCenterPoint.x() -
                    lineSize.width() / 2,
                    reticleCenterPoint.y() -
                    lineSize.height() / 2,
                    lineSize.width(),
                    lineSize.height());

        path.addRect(reticleRect);

        path.moveTo(reticleRect.left(),
                    reticleRect.center().y());

        path.lineTo(reticleCenterPoint.x() -
                    reticleSize.width() / 2,
                    reticleRect.center().y());

        path.moveTo(reticleRect.right() + 1,
                    reticleRect.center().y());

        path.lineTo(reticleCenterPoint.x() +
                    reticleSize.width() / 2,
                    reticleRect.center().y());

        path.moveTo(reticleRect.center().x(),
                    reticleRect.top());

        path.lineTo(reticleRect.center().x(),
                    reticleCenterPoint.y() -
                    reticleSize.height() / 2);

        path.moveTo(reticleRect.center().x(),
                    reticleRect.bottom() + 1);

        path.lineTo(reticleRect.center().x(),
                    reticleCenterPoint.y() +
                    reticleSize.height() / 2);

        break;
    }
    }


    painter->drawPath(path);
}
