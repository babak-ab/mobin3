#include "reticle.h"

Reticle::Reticle(QQuickPaintedItem* parent)
    : QQuickPaintedItem(parent)
{

    setFlag(QQuickItem::ItemHasContents);

    m_reticleColor = Qt::blue;
    m_thickness = 1;
    m_xOffset = 0;
    m_yOffset = 0;
}

void Reticle::paint(QPainter* painter)
{

    qDebug() << "XXXXXXXXXX paint";

    painter->setRenderHint(QPainter::Antialiasing);
    // painter->translate(width() / 2, height() / 2);
    painter->setBrush(Qt::NoBrush);
    QPen pen(Qt::white);
    pen.setWidth(m_thickness);
    pen.setStyle(Qt::DashDotLine);
    painter->setPen(pen);

    QFontMetrics fm(painter->font());
    int pixelsHigh = fm.height();

    painter->fillRect(0, 0, 20, 4, m_reticleColor);
    painter->fillRect(width() - 20, 0, 20, 4, m_reticleColor);

    painter->fillRect(0, 0, 4, 20, m_reticleColor);
    painter->fillRect(0, height() - 20, 4, 20, m_reticleColor);

    painter->fillRect(0, height() - 4, 20, 4, m_reticleColor);
    painter->fillRect(width() - 20, height() - 4, 20, 4, m_reticleColor);

    painter->fillRect(width() - 4, 0, 4, 20, m_reticleColor);
    painter->fillRect(width() - 4, height() - 20, 4, 20, m_reticleColor);

    painter->fillRect(width() / 2 - 8, height() / 2 - 2, 16, 4, m_reticleColor);
    painter->fillRect(width() / 2 - 2, height() / 2 - 8, 4, 16, m_reticleColor);
}

void Reticle::setReticleColor(quint8 color)
{


    switch (color) {
    case 0:
    {
        m_reticleColor = Qt::blue;
        setFlag(QQuickItem::ItemHasContents);
        update();

        break;
    }
    case 1:
    {
        m_reticleColor = Qt::red;
        setFlag(QQuickItem::ItemHasContents);
        update();
        qDebug() << "xxxxxxx: " << color;

        break;
    }
    case 2:
    {
        m_reticleColor = Qt::green;
        setFlag(QQuickItem::ItemHasContents);
        update();

        break;
    }
    }

    Q_EMIT sigDataChanged();

}

void Reticle::setReticleSize(quint8 thickness)
{
    m_thickness = thickness;

    Q_EMIT sigDataChanged();
}

void Reticle::moveReticlePosition(quint16 xOffset, quint16 yOffset)
{
    m_xOffset = xOffset;
    m_yOffset = yOffset;

    Q_EMIT sigDataChanged();
}

quint8 Reticle::color() const
{
    if (m_reticleColor == Qt::blue) {
        return 0;
    } else if (m_reticleColor == Qt::red) {
        return 1;
    } else if (m_reticleColor == Qt::green) {
         return 2;
    } else {
        return 0;
    }
}

