// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MaintenanceResolver_h_
#define __MaintenanceResolver_h_

#include "ConsignData_ABC.h"
#include "ConsignResolver_ABC.h"

namespace plugins
{
namespace logistic
{
// =============================================================================
/** @class  MaintenanceConsignData
    @brief  MaintenanceConsignData
*/
// Created: MMC 2012-08-06
// =============================================================================
class MaintenanceConsignData : public ConsignData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceConsignData( const std::string& requestId ) : ConsignData_ABC( requestId ) {}
    virtual ~MaintenanceConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void WriteConsign( ConsignWriter& output ) const;
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogMaintenanceHandlingCreation& msg, ConsignResolver_ABC& resolver );
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogMaintenanceHandlingUpdate& msg, ConsignResolver_ABC& resolver );
    //@}

public:
    //! @name Member data
    //@{
        std::string creationTick_;
        std::string stateEndTick_;
        std::string unitId_;
        std::string providerId_;
        std::string equipmentId_;
        std::string breakdownId_;
        std::string stateId_;
        std::string unit_;
        std::string provider_;
        std::string equipment_;
        std::string breakdown_;
        std::string state_;
    //@}
};

// =============================================================================
/** @class  MaintenanceResolver
    @brief  MaintenanceResolver
*/
// Created: MMC 2012-08-06
// =============================================================================
class MaintenanceResolver : public ConsignResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceResolver( const tools::Path& name, const NameResolver_ABC& nameResolver );
    virtual ~MaintenanceResolver();
    //@}

    //! @name Operations
    //@{
    virtual void InitHeader();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& message );
    virtual bool IsEmptyLineMessage( const sword::SimToClient& message );
    virtual void ManageMessage( const sword::SimToClient& message );
    virtual ConsignData_ABC* CreateConsignData( int requestId );
    //@}
};
}
}

#endif // __MaintenanceResolver_h_
