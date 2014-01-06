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
#include <vector>

namespace tools
{
    class SessionConfig;
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

class ConsignCsvLogger;
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
             LogisticPlugin( const boost::shared_ptr< ConsignCsvLogger >& logger,
                 const tools::Path& archiveFile, bool load );
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
    virtual bool HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    virtual bool HandleClientToReplay( const sword::ClientToReplay& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    //@}

private:
    template< typename M, typename R >
    bool HandleClientToSomething( const M& msg, dispatcher::RewritingPublisher_ABC& unicaster,
            dispatcher::ClientPublisher_ABC& );
    void HandleLogisticHistoryRequest( const sword::LogisticHistoryRequest& rq,
            sword::LogisticHistoryAck& ack ) const;
    void HandleListLogisticRequests( const sword::ListLogisticRequests& rq,
            sword::ListLogisticRequestsAck& ack ) const;

private:
    //! @name Member data
    //@{
    std::unique_ptr< ConsignIndex >             index_;
    std::unique_ptr< ConsignRecorder >          recorder_;
    boost::shared_ptr< ConsignCsvLogger >       logger_;
    int currentTick_;
    std::string simTime_;
    const bool readOnly_;
    //@}
};

boost::shared_ptr< LogisticPlugin > CreateLogisticPlugin(
    const dispatcher::Model_ABC& model,
    const kernel::StaticModel& staticModel,
    const tools::SessionConfig& config );

boost::shared_ptr< LogisticPlugin > ReloadLogisticPlugin(
    const tools::SessionConfig& config );

}  // namespace logistic
}  // namespace plugins

#endif // __LogisticPlugin_h_
