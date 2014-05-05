
// SupplyResolver.h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SupplyResolver_h_
#define __SupplyResolver_h_

#include "ConsignData_ABC.h"
#include <map>

namespace sword
{
    class LogSupplyHandlingCreation;
    class LogSupplyHandlingUpdate;
}

namespace plugins
{
namespace logistic
{
// =============================================================================
/** @class  SupplyConsignData
    @brief  SupplyConsignData
*/
// Created: MMC 2012-08-06
// =============================================================================
class SupplyConsignData : public ConsignData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
        SupplyConsignData( const std::string& requestId )
                 : ConsignData_ABC( LogisticPlugin::eLogisticType_Supply, requestId ) {}
        virtual ~SupplyConsignData() {}
    //@}

private:
    virtual bool DoUpdateConsign( const sword::SimToClient& msg, const NameResolver_ABC& resolver );
    virtual void WriteConsign( ConsignWriter& output ) const;
    bool ManageMessage( const ::sword::LogSupplyHandlingCreation& msg,
            const NameResolver_ABC& resolver );
    bool ManageMessage( const ::sword::LogSupplyHandlingUpdate& msg, 
            const NameResolver_ABC& resolver );

public:

    //! @name Types
    //@{
    struct Resource
    {
        Resource() : recipientAutomatId_( 0 ) {}
        virtual ~Resource() {}

        int recipientAutomatId_;
        std::string recipientId_;
        std::string type_;
        std::string id_;
        std::string requested_;
        std::string granted_;
        std::string conveyed_;
    };
    //@}

public:
    //! @name Member data
    //@{
    std::string creationTick_;
    std::string stateEndTick_;
    std::map< int, std::string > recipientAutomats_;
    std::string recipientAutomatId_;
    std::string providerId_;
    std::string transportProviderId_;
    std::string conveyorId_;
    std::string stateId_;
    std::string recipientAutomat_;
    std::string provider_;
    std::string transportProvider_;
    std::string conveyor_;
    std::string state_;
    std::map< std::pair< int, int >, Resource > resources_;
    //@}
};

std::string GetSupplyHeader();

}
}

#endif // __SupplyResolver_h_
