#ifndef VIDEOADAPTER_H
#define VIDEOADAPTER_H

#include <QAbstractVideoSurface>
#include <QObject>
#include <QPointer>
#include <QQmlEngine>
#include <QTimer>
#include <QVideoSurfaceFormat>
class  VideoAdapter : public QObject {
    Q_OBJECT

    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)
    Q_PROPERTY(QSize frameSize READ frameSize NOTIFY sizeChanged)

public:
    explicit VideoAdapter(QSize frameSize = QSize(640, 480), QObject* parent = Q_NULLPTR);

    virtual ~VideoAdapter();
    QAbstractVideoSurface* getVideoSurface() const;
    void setVideoSurface(QAbstractVideoSurface* surface);

private:
    QAbstractVideoSurface* m_videoSurface = Q_NULLPTR;
    QVideoFrame* m_videoFrame = Q_NULLPTR;
    QSize m_frameSize;
    void update();

Q_SIGNALS:
    void sizeChanged();

public:
    void onFrameReady(const QByteArray& data);
    void setFrameSize(const QSize size);
    QSize frameSize() const;
    QSize getFrameSize() const;
};

#endif // VIDEOADAPTER_H
