// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogMaintenanceEquipments_h_
#define __LogMaintenanceEquipments_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  LogMaintenanceEquipments
    @brief  LogMaintenanceEquipments
*/
// Created: FPO 2011-05-04
// =============================================================================
class LogMaintenanceEquipments : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 LogMaintenanceEquipments();
    /*implicit*/ LogMaintenanceEquipments( xml::xistream& xis );
    virtual     ~LogMaintenanceEquipments();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_log_maintenance_state() && (
            wrapper.message().log_maintenance_state().has_repairers() ||
            wrapper.message().log_maintenance_state().has_haulers() ) );
    }
    int Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Types
    //@{
    struct Equip
    {
        Equip( bool isAsked = false ) : isAsked_( isAsked ), quantity_( 0 ) { }
        bool isAsked_;
        int quantity_;
    };
    //@}

private:
    //! @name Member data
    //@{
    Equip repairers_;
    Equip haulers_;
    //@}
};

}

#endif // __LogMaintenanceEquipments_h_
