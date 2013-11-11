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
             MaintenanceConsignData( const std::string& requestId )
                 : ConsignData_ABC( LogisticPlugin::eLogisticType_Maintenance, requestId ) {}
    virtual ~MaintenanceConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual bool UpdateConsign( const sword::SimToClient& msg, const NameResolver_ABC& resolver );
    //@}

private:
    virtual void WriteConsign( ConsignWriter& output ) const;
    void ManageMessage( const ::sword::LogMaintenanceHandlingCreation& msg, 
            const NameResolver_ABC& resolver );
    void ManageMessage( const ::sword::LogMaintenanceHandlingUpdate& msg, 
            const NameResolver_ABC& resolver );

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

std::string GetMaintenanceHeader();

}
}

#endif // __MaintenanceResolver_h_
