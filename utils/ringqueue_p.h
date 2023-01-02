#include <QDebug>
#include <QMutex>
#include <QSemaphore>
#include <QString>
#include <QThread>
#include <QWaitCondition>
#include <ringqueue.h>

namespace Utils {

namespace Internal {

template <class T>
struct RingQueueNode {
    T Data;
    RingQueueNode* Next;

public:
    explicit RingQueueNode(T data)
    {
        Data = data;
        Next = Q_NULLPTR;
    }
    RingQueueNode(T data, RingQueueNode* next)
    {
        Data = data;
        Next = next;
    }
};

class RingQueueThread : public QThread {
    Q_OBJECT
    void run() Q_DECL_OVERRIDE
    {
        mutex.lock();
        bool loop = true;
        mutex.unlock();
        while (loop) {
            Q_EMIT resultReady();
            usleep(_interval);
            mutex.lock();
            if (exit)
                loop = false;
            mutex.unlock();
        }
    }

public:
    explicit RingQueueThread(QString name, uint interval = 0, QObject* parent = Q_NULLPTR)
        : QThread(parent)
    {
        _name = name;
        _interval = interval;
        exit = false;
    }

    void dispose()
    {
        mutex.lock();
        exit = true;
        mutex.unlock();
    }

    QString _name;
    uint _interval;
    bool exit;
    QMutex mutex;
Q_SIGNALS:
    void resultReady();
};

template <class T>
class RQueue : public IRQueue<T> {
public:
    explicit RQueue(QString name = "NoName", quint32 capacity = 102400)
        : sem(1)
        , mutex()
    {
        mQueueName = name;
        mCapacity = capacity;
        first = last = Q_NULLPTR;
        mCount = 0;
        quit = false;
    }
    virtual ~RQueue() {}
    void setName(QString name)
    {
        mQueueName = name;
    }
    QString name()
    {
        return mQueueName;
    }
    quint32 count()
    {
        quint32 value;
        mutex.lock();
        value = mCount;
        mutex.unlock();
        return value;
    }
    void dispose()
    {
        mutex.lock();
        quit = true;
        cond.wakeAll();
        mutex.unlock();
        clear();
    }
    void clear()
    {
        while (first != last) {
            RingQueueNode<T>* tempLink = first;
            if (first->Next != 0)
                first = first->Next;
            delete tempLink;
            tempLink = Q_NULLPTR;
        }
        last = Q_NULLPTR;
        first = Q_NULLPTR;
    }
    void enqueue(T* item, int count) override
    {
        for (int i = 0; i < count && !quit; i++) {
            RingQueueNode<T>* link = new RingQueueNode<T>(item[i]);
            sem.acquire();
            if (mCount == 0) {
                last = first = link;
            } else {
                last->Next = link;
                last = link;
            }
            mCount++;
            sem.release();
            mutex.lock();
            cond.wakeOne();
            mutex.unlock();
        }
    }
    void enqueue(T item) override
    {
        RingQueueNode<T>* link = new RingQueueNode<T>(item);
        sem.acquire();
        if (mCount == 0) {
            last = first = link;
        } else {
            last->Next = link;
            last = link;
        }
        mCount++;
        sem.release();
        mutex.lock();
        cond.wakeOne();
        mutex.unlock();
    }
    bool dequeue(T& item) override
    {
        mutex.lock();
        while (mCount == 0 && !quit) {
            cond.wait(&mutex);
        }
        mutex.unlock();
        if (quit == true)
            return false;
        RingQueueNode<T>* tempLink = first;
        sem.acquire();
        if (mCount == 1) {
            item = tempLink->Data;
        } else {
            first = first->Next;
            item = tempLink->Data;
        }
        delete tempLink;
        tempLink = Q_NULLPTR;
        mCount--;
        sem.release();
        return true;
    }
    bool dequeue(T* items, quint32 count) override
    {
        mutex.lock();
        while (count > mCount && !quit) {
            cond.wait(&mutex);
        }
        mutex.unlock();
        if (quit == true)
            return false;
        for (quint32 i = 0; i < count && !quit; i++) {
            RingQueueNode<T>* tempLink = first;
            sem.acquire();
            mCount--;
            items[i] = tempLink->Data;
            if (mCount > 0) {
                first = first->Next;
            }
            sem.release();
            delete tempLink;
            tempLink = Q_NULLPTR;
            if (mCount == 0)
                break;
        }
        if (quit)
            return false;
        return true;
    }
    bool empty()
    {
        bool value;
        mutex.lock();
        if (mCount == 0)
            value = false;
        else
            value = true;
        mutex.unlock();
        return value;
    }

private:
    QSemaphore sem;
    RingQueueNode<T>* first;
    RingQueueNode<T>* last;
    quint32 mCount;
    QMutex mutex;
    QWaitCondition cond;
    quint32 mCapacity;
    QString mQueueName;
    bool quit;
};

class RingQueuePrivate : public QObject {
    Q_OBJECT
public:
    RingQueuePrivate(RingQueue* ringqueue, QString name, int intervalRead = 10,int  intervalWrite = 10 ,QObject* parent = Q_NULLPTR);
    virtual ~RingQueuePrivate();
    void insertToWriteQueue(QByteArray data);
    void insertToReadQueue(QByteArray data);
    void initialize();
    void finalize();
    int count();

private Q_SLOTS:
    void sltThreadWriteResult();
    void sltThreadReadResult();

private:
    RingQueue* q;
    RingQueueThread* mThreadRead;
    RQueue<quint8> mRingQueueRead;
    RingQueueThread* mThreadWrite;
    RQueue<QByteArray> mRingQueueWrite;
    QString m_name;
    int m_inteval;
};
}
}
