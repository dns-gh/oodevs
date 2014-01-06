// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTICPLUGIN_CONSIGNCSVLOGGER_H
#define LOGISTICPLUGIN_CONSIGNCSVLOGGER_H

#include "ConsignIndex.h"
#include "LogisticPlugin.h"
#pragma warning( push, 0 )
#include <boost/ptr_container/ptr_map.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <string>

class QApplication;

namespace boost
{
    namespace gregorian
    {
        class date;
    }
}

namespace tools
{
    class Path;
    class SessionConfig;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace kernel
{
    class StaticModel;
}

namespace plugins
{
namespace logistic
{

class ConsignResolver_ABC;
class NameResolver_ABC;

class ConsignCsvLogger: private boost::noncopyable
{
public:
    ConsignCsvLogger( const boost::shared_ptr< const NameResolver_ABC >& resolver,
        const tools::Path& maintenanceFile, const tools::Path& supplyFile,
        const tools::Path& funeralFile, const tools::Path& medicalFile );
    virtual ~ConsignCsvLogger();

    void Log( const ConsignEvent& ev, const sword::SimToClient& message,
           const boost::gregorian::date& today, int32_t tick,
           const std::string& simTime  );
    void Write( int kind, const std::string& data, const boost::gregorian::date& today );
    void Flush();
    void SetMaxLinesInFile( int maxLines );
    int DebugGetConsignCount( LogisticPlugin::E_LogisticType eLogisticType ) const;

private:
    boost::ptr_map< int, ConsignResolver_ABC >  loggers_;
    boost::ptr_map< int, ConsignData_ABC >      consigns_;
    std::unique_ptr< QApplication >             localAppli_;
    boost::shared_ptr< const NameResolver_ABC > nameResolver_;
};

boost::shared_ptr< ConsignCsvLogger > CreateCsvLogger(
    const dispatcher::Model_ABC& model,
    const kernel::StaticModel& staticModel,
    const tools::SessionConfig& config );

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNCSVLOGGER_H
