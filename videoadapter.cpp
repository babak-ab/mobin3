#include "videoadapter.h"
#include <QDateTime>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QRandomGenerator>
#include <QSize>
#include <QVideoFrame>

VideoAdapter::VideoAdapter(QSize frameSize, QObject* parent)
    : QObject(parent)
{
    setFrameSize(frameSize);
}

VideoAdapter::~VideoAdapter()
{
    if (m_videoFrame != Q_NULLPTR) {
        delete m_videoFrame;
        m_videoFrame = Q_NULLPTR;
    }
}

QAbstractVideoSurface* VideoAdapter::getVideoSurface() const
{
    return m_videoSurface;
}

void VideoAdapter::setVideoSurface(QAbstractVideoSurface* surface)
{
    if (m_videoSurface != surface) {
        m_videoSurface = surface;
        if (m_videoSurface) {
            update();
        }
    }
}

QSize VideoAdapter::getFrameSize() const
{
    return m_frameSize;
}

QSize VideoAdapter::frameSize() const
{
    return m_frameSize;
}

void VideoAdapter::onFrameReady(const QByteArray& data)
{
    if (m_videoFrame == Q_NULLPTR)
        return;

    if (m_videoSurface == Q_NULLPTR)
        return;

    if (m_videoFrame && m_videoSurface) {
        memcpy(m_videoFrame->bits(), data.data(), m_frameSize.width() * m_frameSize.height() * 4);
    }
    if (m_videoSurface && m_videoFrame) {
        m_videoSurface->present(*m_videoFrame);
    }
}

void VideoAdapter::setFrameSize(const QSize size)
{
    m_frameSize = size;
    update();
    Q_EMIT sizeChanged();
}

void VideoAdapter::update()
{
    if (m_videoSurface) {
        if (m_videoFrame && m_videoFrame->isMapped())
            m_videoFrame->unmap();

        if (m_videoFrame) {
            delete m_videoFrame;
            m_videoFrame = Q_NULLPTR;
        }

        if (m_videoSurface) {
            QImage rgbA = QImage(m_frameSize.width(), m_frameSize.height(), QImage::Format_RGB32);
            rgbA.fill(Qt::black);
            m_videoFrame = new QVideoFrame(rgbA);
        }
        if (m_videoFrame)
            m_videoFrame->map(QAbstractVideoBuffer::ReadOnly);

        if (m_videoSurface->isActive())
            m_videoSurface->stop();

        m_videoSurface->start(QVideoSurfaceFormat(m_frameSize, QVideoFrame::Format_RGB32));

        if (m_videoSurface && m_videoFrame) {
            m_videoSurface->present(*m_videoFrame);
        }
    }
}
