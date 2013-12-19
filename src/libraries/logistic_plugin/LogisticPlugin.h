// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __LogisticPlugin_h_
#define __LogisticPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#include <boost/ptr_container/ptr_map.hpp>
#pragma warning( pop )
#include <vector>

class QApplication;
namespace tools
{
    class SessionConfig;
}

namespace xml
{
    class xistream;
}

namespace boost
{
    namespace gregorian
    {
        class date;
    }
}

namespace sword
{
    class LogisticHistoryRequest;
    class LogisticHistoryAck;
    class ListLogisticRequests;
    class ListLogisticRequestsAck;
}

namespace tools
{
    class Path;
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

class ConsignData_ABC;
class ConsignIndex;
class ConsignRecorder;
class NameResolver_ABC;

// =============================================================================
/** @class  LogisticPlugin
    @brief  LogisticPlugin
*/
// Created: MMC 2012-08-06
// =============================================================================
class LogisticPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticPlugin(const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
                 const tools::Path& archiveFile, const tools::Path& maintenanceFile,
                 const tools::Path& supplyFile, const tools::Path& funeralFile,
                 const tools::Path& medicalFile );
    virtual ~LogisticPlugin();
    //@}

    //! @name Types
    //@{
    enum E_LogisticType
    {
        eLogisticType_Maintenance,
        eLogisticType_Supply,
        eLogisticType_Funeral,
        eLogisticType_Medical,
        eNbrLogisticType
    };
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    virtual void Receive( const sword::SimToClient& message, const boost::gregorian::date& today );
    virtual bool LogisticPlugin::HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    int DebugGetConsignCount( E_LogisticType eLogisticType ) const;
    void SetMaxLinesInFile( int maxLines );
    //@}

private:
    void HandleLogisticHistoryRequest( const sword::LogisticHistoryRequest& rq,
            sword::LogisticHistoryAck& ack ) const;
    void HandleListLogisticRequests( const sword::ListLogisticRequests& rq,
            sword::ListLogisticRequestsAck& ack ) const;

private:
    //! @name Member data
    //@{
    std::unique_ptr< ConsignIndex >             index_;
    std::unique_ptr< ConsignRecorder >          recorder_;
    std::unique_ptr< QApplication >             localAppli_;
    boost::shared_ptr<const NameResolver_ABC>   nameResolver_;
    boost::ptr_map< int, ConsignData_ABC >      consigns_;
    int currentTick_;
    std::string simTime_;
    //@}
};

boost::shared_ptr< LogisticPlugin > CreateLogisticPlugin(
    const dispatcher::Model_ABC& model,
    const kernel::StaticModel& staticModel,
    const tools::SessionConfig& config );

}
}

#endif // __LogisticPlugin_h_
