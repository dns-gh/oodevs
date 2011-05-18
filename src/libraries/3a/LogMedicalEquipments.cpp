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

    bool ReadMask( xml::xistream& xis, std::string equipment )
    {
        if( !xis.has_attribute( "types" ) )
            return true;
        FilterHelper< std::string > equipments( xis, "types" );
        bool result = false;
        if( equipments.IsAllowed( equipment ) )
                result = true;
        return result;
    }

    int addToResult( const sword::SeqOfLogMedicalEquipmentAvailability& equipments )
    {
        int result = 0;
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
    : ambulances_releve_ ( ReadMask ( xis, "releve" ) )
    , ambulances_ramassage_ ( ReadMask ( xis, "ramassage" ) )
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
int LogMedicalEquipments::Extract( const sword::SimToClient& wrapper )
{
    const sword::LogMedicalState& logstate = wrapper.message().log_medical_state();
    int result = 0;

    if( ambulances_releve_.isAsked_ && logstate.has_evacuation_ambulances() )
    {
        const sword::SeqOfLogMedicalEquipmentAvailability& equipments = logstate.evacuation_ambulances();
        ambulances_releve_.quantity_ = addToResult( equipments );
    }
    if( ambulances_ramassage_.isAsked_ && logstate.has_collection_ambulances() )
    {
        const sword::SeqOfLogMedicalEquipmentAvailability& equipments = logstate.collection_ambulances();
        ambulances_ramassage_.quantity_ = addToResult( equipments );
    }
    result += ambulances_releve_.quantity_;
    result += ambulances_ramassage_.quantity_;
    return result;
}
