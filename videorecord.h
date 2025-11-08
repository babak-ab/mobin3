#ifndef GSTUDPRECORD_H
#define GSTUDPRECORD_H

#include <QColor>
#include <QDir>
#include <QImage>
#include <QMetaType>
#include <QDebug>
#include <QObject>
#include <QRgb>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QUrl>

#include <gst/gst.h>

#include <memory>
#include <iostream>

///
/// \brief this class is responsible for receiving ogab's camera's output frames and
/// creates a record file of what oghab sees.
///
class VideoRecord : public QObject {
    Q_OBJECT

public:
    explicit VideoRecord(QObject* parent = Q_NULLPTR);
    explicit VideoRecord(QString dirName, QObject* parent = Q_NULLPTR);
    virtual ~VideoRecord();
    // bitrate Kbit/sec 1000 ~ 1000000 bit/sec

    void startRecord();
    void stopRecord();
    void pause();

    QString videoLocation() const;

    void setVideoLocation(
            const QString& videoLocation);

    QString videoFileName();

    bool isRecording() const;

private:
    QString m_currentVideoFileName;
    bool m_recordStatus;
    bool m_needdata;
    quint32 m_bitrate;
    bool m_isFirstRun;

    struct Elements {
        Elements()
        {
            pipeline = Q_NULLPTR;
            appsrc = Q_NULLPTR;
            videoencoder = Q_NULLPTR;
            bus = Q_NULLPTR;
        }
        GstElement* pipeline;
        GstElement* appsrc;
        GstElement* videoencoder;
        GstBus* bus;
    };

    Elements m_elements;
    QString m_videoLocation;
    GstBuffer* m_buffer;
    int m_waitBeforeNoVideo;
    static GstBusSyncReply onBusMessageVideoPipeline(
            GstBus* bus, GstMessage* msg, gpointer user_data);

    template <typename T>
    static std::shared_ptr<T> takeGstMiniObject(T* o);

    QString videoName();
    void createNewVideoWriter();

private Q_SLOTS:
    void busMessage(std::shared_ptr<GstMessage> message);

Q_SIGNALS:
    void sigError(QString src, QString msg, QString desc);
    void sigEos();
    void sigRecordingStatusChanged();
    void sigRecordingSaved(const QString& filePath, const QString& label);
};

#endif // GSTRECORD_H
