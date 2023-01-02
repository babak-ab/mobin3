#ifndef GLOBAL_H
#define GLOBAL_H
#include <QImage>

class IObserver {
public:
    virtual ~IObserver() {};    
    virtual void onReadyRead(QString name, uchar* data, long size) = 0;

};
class ISubject {
public:
    ISubject(QString name) { _name = name; }
    virtual ~ISubject() {};
    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void detachAll() = 0;
    virtual void notify(uchar* data, long size) = 0;


protected:
    QString _name;
};

#endif // GLOBAL_H
