
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
#include "ConsignResolver_ABC.h"

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
        SupplyConsignData( const std::string& requestId ) : ConsignData_ABC( requestId ) {}
        virtual ~SupplyConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void operator>>( std::stringstream& output ) const;
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogSupplyHandlingCreation& msg, ConsignResolver_ABC& resolver );
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogSupplyHandlingUpdate& msg, ConsignResolver_ABC& resolver );
    //@}

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
    std::string tick_;
    std::string creationTick_;
    std::string stateEndTick_;
    std::map< int, std::string > recipientAutomats_;
    std::string recipientAutomatId_;
    std::string providerId_;
    std::string transportProviderId_;
    std::string conveyorId_;
    std::string stateId_;
    std::string simTime_;
    std::string recipientAutomat_;
    std::string provider_;
    std::string transportProvider_;
    std::string conveyor_;
    std::string state_;
    std::map< int, Resource > resources_;
    //@}
};

// =============================================================================
/** @class  SupplyResolver
    @brief  SupplyResolver
*/
// Created: MMC 2012-08-06
// =============================================================================
class SupplyResolver : public ConsignResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResolver( const std::string& name, const NameResolver_ABC& nameResolver );
    virtual ~SupplyResolver();
    //@}

    //! @name Operations
    //@{
    virtual void InitHeader();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& message );
    virtual void ManageMessage( const sword::SimToClient& message );
    virtual ConsignData_ABC* CreateConsignData( int requestId );
    //@}
};
}
}

#endif // __SupplyResolver_h_