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
#include "ConsignResolver_ABC.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

namespace tools
{
    class SessionConfig;
}

namespace dispatcher
{
    class ClientsNetworker;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace logistic
{

class ConsignResolver_ABC;
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
             LogisticPlugin( const tools::SessionConfig& config,
                 const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
                 int maxHistoricFiles, int maxFileLines, dispatcher::ClientsNetworker& clients );
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
    virtual int GetConsignCount( E_LogisticType eLogisticType ) const;
    virtual void SetMaxLinesInFile( int maxLines );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticPlugin( const LogisticPlugin& );            //!< Copy constructor
    LogisticPlugin& operator=( const LogisticPlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void OnReceive( const sword::ClientToSim& message );
    void OnReceiveClientToMessenger( const sword::ClientToMessenger& message );
    void ClearFilesWhenLoadingCheckpoint( const std::string& checkpointDirectory );
    void SaveCheckpoint( const std::string& checkpointDirectory );
    void UpdateCurrentTick( int currentTick, const std::string& date );
    //@}

    //! @name Member data
    //@{    
    std::vector< ConsignResolver_ABC* > resolvers_;
    boost::shared_ptr<const NameResolver_ABC> nameResolver_;
    dispatcher::ClientsNetworker& clients_;
    int currentTick_;
    const tools::SessionConfig& config_;
    //@}
};

LogisticPlugin* CreateLogisticPlugin( const tools::SessionConfig& config, const boost::shared_ptr<const NameResolver_ABC>& nameResolver, int maxHistoricFiles, int maxFileLines, dispatcher::ClientsNetworker& clients );

}
}

#endif // __LogisticPlugin_h_
