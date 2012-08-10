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
             MaintenanceConsignData();
    virtual ~MaintenanceConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void Write( std::ofstream& output );
    //@}

public:
    //! @name Member data
    //@{
        int tick_;
        int unitId_;
        int providerId_;
        int equipmentId_;
        int breakdownId_;
        int stateId_;
        std::string simTime_;
        std::string unit_;
        std::string provider_;
        std::string equipment_;
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
             MaintenanceResolver( const std::string& name, const dispatcher::Model_ABC& model );
    virtual ~MaintenanceResolver();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& message );
    virtual void ManageMessage( const sword::SimToClient& message );
    //@}
};
}
}

#endif // __MaintenanceResolver_h_