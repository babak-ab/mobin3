#include "utils.h"
#include <QDebug>
using namespace Utils;
Tester* Tester::m_instance = Q_NULLPTR;
Tester::Tester(QObject* parent)
    : QObject(parent)
{
    m_instance = this;
}

Tester* Tester::instance()
{
    if (m_instance == Q_NULLPTR) {
        m_instance = new Tester;
    }
    return m_instance;
}

qint64 Tester::elapseTime()
{
    if (!m_elapsedTimer.isValid())
        m_elapsedTimer.start();
    qint64 diff = m_elapsedTimer.elapsed() - m_elapsed;
    m_elapsed = m_elapsedTimer.elapsed();
    return diff;
}
