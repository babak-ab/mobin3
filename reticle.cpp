#include "reticle.h"

Reticle::Reticle(QQuickPaintedItem* parent)
    : QQuickPaintedItem(parent)
{

    //setFlag(QQuickItem::ItemHasContents);

    m_reticleColor = QColor(66, 173, 245);
    m_thickness = 8;
    m_xOffset = 0;
    m_yOffset = 0;
}

void Reticle::paint(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    // painter->translate(width() / 2, height() / 2);
    painter->setBrush(Qt::NoBrush);
    QPen pen(Qt::white);
    pen.setWidth(m_thickness);
    painter->setPen(pen);

    QFontMetrics fm(painter->font());
    int pixelsHigh = fm.height();

    painter->fillRect(0, 0, 20, m_thickness, m_reticleColor);
    painter->fillRect(width() - 20, 0, 20, m_thickness, m_reticleColor);

    painter->fillRect(0, 0, m_thickness, 20, m_reticleColor);
    painter->fillRect(0, height() - 20, m_thickness, 20, m_reticleColor);

    painter->fillRect(0, height() - m_thickness, 20, m_thickness, m_reticleColor);
    painter->fillRect(width() - 20, height() - m_thickness, 20, m_thickness, m_reticleColor);

    painter->fillRect(width() - m_thickness, 0, m_thickness, 20, m_reticleColor);
    painter->fillRect(width() - m_thickness, height() - 20, m_thickness, 20, m_reticleColor);

    painter->fillRect(width() / 2 - 2 * m_thickness, height() / 2 - m_thickness / 2.0, 4 * m_thickness, m_thickness, m_reticleColor);
    painter->fillRect(width() / 2 - m_thickness / 2.0, height() / 2 - 2 * m_thickness, m_thickness, 4 * m_thickness, m_reticleColor);
}

void Reticle::setReticleColor(quint8 color)
{
    switch (color) {
    case 0:
    {
        m_reticleColor = Qt::blue;
        update();

        break;
    }
    case 1:
    {
        m_reticleColor = Qt::red;
        update(QRect(0, 0, 1920, 1080));

        break;
    }
    case 2:
    {
        m_reticleColor = Qt::green;
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

