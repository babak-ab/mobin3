#ifndef RINGQUEUE_H
#define RINGQUEUE_H

#include "utils_global.h"
#include <QObject>

namespace Utils {

namespace Internal {
    class RingQueuePrivate;
}
template <class T>

///
/// \brief The IRQueue class is the interface of handling the queue.
///
class IRQueue {

public:
    virtual void enqueue(T* item, int count) = 0;
    virtual void enqueue(T item) = 0;
    virtual bool dequeue(T& item) = 0;
    virtual bool dequeue(T* items, quint32 count) = 0;
};

///
/// \brief The RingQueue class is responsible for handling the probable queue created in the serail data reception.
///
class RingQueue : public QObject {
    Q_OBJECT
public:
    explicit RingQueue(QString name, int intervalRead = 10, int intervalWrite = 10, QObject* parent = Q_NULLPTR);
    virtual ~RingQueue();
    virtual QByteArray interpret(IRQueue<quint8>* queueRead) = 0;
    void insertToWriteQueue(QByteArray data);
    void insertToReadQueue(QByteArray data);
    void initialize();
    void finalize();
    // Number of Inserted data
    int count();
Q_SIGNALS:
    void sigReadData(QByteArray data);
    void sigWriteData(QByteArray data);

private:
    Internal::RingQueuePrivate* d;
};
}
#endif // RINGQUEUE_H
