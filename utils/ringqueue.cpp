#include <QLabel>
#include <ringqueue.h>
#include <ringqueue_p.h>

#include <iostream>

using namespace Utils;
using namespace Utils::Internal;

RingQueue::RingQueue(QString name, int intervalRead, int intervalWrite, QObject* parent)
{
    d = new RingQueuePrivate(this, name, intervalRead, intervalWrite, parent);
}

RingQueue::~RingQueue()
{
    delete d;
    d = Q_NULLPTR;
}

void RingQueue::insertToWriteQueue(QByteArray data)
{
    d->insertToWriteQueue(data);
}

void RingQueue::insertToReadQueue(QByteArray data)
{
    d->insertToReadQueue(data);
}

void RingQueue::initialize()
{
    d->initialize();
}

void RingQueue::finalize()
{
    d->finalize();
}

int RingQueue::count()
{
    return d->count();
}
//=================================RingQueuePrivare==================

RingQueuePrivate::RingQueuePrivate(RingQueue* ringqueue, QString name, int intervalRead, int intervalWrite, QObject* parent)
    : QObject(parent)
    , q(ringqueue)
{
    m_name = name;
    m_inteval = intervalRead;
    mThreadRead = new RingQueueThread(name, intervalRead, this);
    connect(mThreadRead, &RingQueueThread::resultReady, this,
        &RingQueuePrivate::sltThreadReadResult, Qt::DirectConnection);

    mThreadWrite = new RingQueueThread(name, intervalWrite, this);
    connect(mThreadWrite, &RingQueueThread::resultReady, this,
        &RingQueuePrivate::sltThreadWriteResult, Qt::DirectConnection);

    mRingQueueRead.setName(name + "Read");
    mRingQueueWrite.setName(name + "Write");
}

void RingQueuePrivate::initialize()
{
    mThreadWrite->start();
    mThreadRead->start();
}
void RingQueuePrivate::finalize()
{
    mThreadRead->dispose();
    mThreadWrite->dispose();

    mRingQueueRead.dispose();
    mRingQueueWrite.dispose();

    mThreadRead->quit();
    mThreadWrite->quit();

    mThreadRead->wait();
    mThreadWrite->wait();
}

int RingQueuePrivate::count()
{
    return mRingQueueRead.count();
}

RingQueuePrivate::~RingQueuePrivate()
{
    delete mThreadRead;
    delete mThreadWrite;
    mThreadRead = Q_NULLPTR;
    mThreadWrite = Q_NULLPTR;
}

void RingQueuePrivate::insertToWriteQueue(QByteArray data)
{
    mRingQueueWrite.enqueue(data);
}
void RingQueuePrivate::insertToReadQueue(QByteArray data)
{
    mRingQueueRead.enqueue((quint8*)data.data(), data.length());
}
void RingQueuePrivate::sltThreadReadResult()
{
    QByteArray ba = q->interpret(&mRingQueueRead);

    if (!ba.isEmpty())
        Q_EMIT q->sigReadData(ba);
}
void RingQueuePrivate::sltThreadWriteResult()
{
    QByteArray ba;

    if (mRingQueueWrite.dequeue(ba) == true)
    {
        Q_EMIT q->sigWriteData(ba);
    }
    else
    {
        ba.resize(6);
        ba[0] = 0xff;
        ba[1] = 0x80;
        ba[2] = 0x80;
        ba[3] = 0x00;
        ba[4] = 0x10;
        ba[5] = 0x64;
        Q_EMIT q->sigWriteData(ba);
    }
}
