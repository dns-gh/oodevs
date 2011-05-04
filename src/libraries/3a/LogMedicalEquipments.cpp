// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LogMedicalEquipments.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: LogMedicalEquipments constructor
// Created: FPO 2011-05-03
// -----------------------------------------------------------------------------
LogMedicalEquipments::LogMedicalEquipments()
{
    // NOTHING
}

namespace
{
    const char* logEquipements[ 2 ] =
    {
        "releve",
        "ramassage",
    };

    bool ReadMask( xml::xistream& xis, unsigned int index )
    {
        if( xis.attribute< std::string >( "types" ).empty() )
            return true;
        FilterHelper< std::string > equipments( xis, "types" );
        bool result = false;
        if( equipments.IsAllowed( logEquipements[index] ) )
                result = true;
        return result;
    }

    float addToResult( const sword::SeqOfLogMedicalEquipmentAvailability& equipments )
    {
        float result = 0;
        for( int u = 0; u < equipments.elem_size(); ++u )
        {
            const sword::LogMedicalEquipmentAvailability& equip = equipments.elem( u );
            result += equip.total();
        }
        return result;
    }

}

// -----------------------------------------------------------------------------
// Name: LogMedicalEquipments constructor
// Created: FPO 2011-05-03
// -----------------------------------------------------------------------------
LogMedicalEquipments::LogMedicalEquipments( xml::xistream& xis )
    : ambulances_releve_ ( ReadMask ( xis, 0 ) )
    , ambulances_ramassage_ ( ReadMask ( xis, 1 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMedicalEquipments destructor
// Created: FPO 2011-05-03
// -----------------------------------------------------------------------------
LogMedicalEquipments::~LogMedicalEquipments()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMedicalEquipments::Extract
// Created: FPO 2011-05-03
// -----------------------------------------------------------------------------
float LogMedicalEquipments::Extract( const sword::SimToClient& wrapper ) const
{
    const sword::LogMedicalState& logstate = wrapper.message().log_medical_state();
    float result = 0;

    if( ambulances_releve_ && logstate.has_evacuation_ambulances() )
    {
        const sword::SeqOfLogMedicalEquipmentAvailability& equipments = logstate.evacuation_ambulances();
        result += addToResult( equipments );
    }
    if( ambulances_ramassage_ && logstate.has_collection_ambulances() )
    {
        const sword::SeqOfLogMedicalEquipmentAvailability& equipments = logstate.collection_ambulances();
        result += addToResult( equipments );
    }
    return result;
}
