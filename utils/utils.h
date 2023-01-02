#ifndef UTILS_H
#define UTILS_H

#include "utils_global.h"

#include <QElapsedTimer>
#include <QObject>

namespace Utils {
///
/// \brief The Tester class is designed to test the queuing procedure of the serial data.
///
class Tester : public QObject {
    Q_OBJECT
public:
    explicit Tester(QObject* parent = Q_NULLPTR);
    static Tester* instance();
    qint64 elapseTime();

private:
    static Tester* m_instance;
    QElapsedTimer m_elapsedTimer;
    quint64 m_elapsed;
};
}
#endif // UTILS_H
