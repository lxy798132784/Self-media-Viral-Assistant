/**
 * @file data_source_base.cpp
 * @brief Implementation of DataSourceBase abstract class
 * @brief DataSourceBase 抽象类的实现
 */

#include "api/data_source_base.h"

namespace mva::api {

DataSourceBase::DataSourceBase(QObject *parent)
    : QObject(parent)
{
}

void DataSourceBase::setAvailable(bool available)
{
    if (m_available != available) {
        m_available = available;
        emit availabilityChanged(available);
    }
}

void DataSourceBase::setError(DataSourceError error, const QString &message)
{
    m_lastError = message;
    qWarning() << "[DataSource]" << providerName() 
               << "Error:" << static_cast<int>(error) << message;
    emit errorOccurred(error, message);
}

} // namespace mva::api
