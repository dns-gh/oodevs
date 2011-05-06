// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LogMaintenanceEquipments.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: LogMaintenanceEquipments constructor
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
LogMaintenanceEquipments::LogMaintenanceEquipments()
{
    // NOTHING
}

namespace
{
    const char* logEquipements[ 2 ] =
    {
        "repairers",
        "haulers",
    };

    bool ReadMask( xml::xistream& xis, unsigned int index )
    {
        if( !xis.has_attribute( "types" ) )
            return true;
        FilterHelper< std::string > equipments( xis, "types" );
        bool result = false;
        if( equipments.IsAllowed( logEquipements[index] ) )
            result = true;
        return result;
    }

    float addToResult( const sword::SeqOfLogMaintenanceEquipmentAvailability& equipments )
    {
        float result = 0;
        for( int u = 0; u < equipments.elem_size(); ++u )
        {
            const sword::LogMaintenanceEquipmentAvailability& equip = equipments.elem( u );
            result += equip.total();
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceEquipments constructor
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
LogMaintenanceEquipments::LogMaintenanceEquipments( xml::xistream& xis )
    : repairers_ ( ReadMask ( xis, 0 ) )
    , haulers_ ( ReadMask ( xis, 1 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceEquipments destructor
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
LogMaintenanceEquipments::~LogMaintenanceEquipments()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceEquipments::Extract
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
float LogMaintenanceEquipments::Extract( const sword::SimToClient& wrapper )
{
    const sword::LogMaintenanceState& logstate = wrapper.message().log_maintenance_state();
    float result = 0;

    if( repairers_.isAsked_ && logstate.has_repairers() )
    {
        const sword::SeqOfLogMaintenanceEquipmentAvailability& equipments = logstate.repairers();
        repairers_.quantity_ = addToResult( equipments );
    }
    if( haulers_.isAsked_ && logstate.has_haulers() )
    {
        const sword::SeqOfLogMaintenanceEquipmentAvailability& equipments = logstate.haulers();
        haulers_.quantity_ = addToResult( equipments );
    }
    result += repairers_.quantity_;
    result += haulers_.quantity_;
    return result;
}
