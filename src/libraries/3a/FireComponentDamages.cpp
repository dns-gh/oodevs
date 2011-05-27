// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "FireComponentDamages.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: FireComponentDamages constructor
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
FireComponentDamages::FireComponentDamages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireComponentDamages destructor
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
FireComponentDamages::~FireComponentDamages()
{
    // NOTHING
}

namespace
{
    const unsigned nEquipmentStates = 3;
    typedef google::protobuf::uint32( UnitEquipmentFireDamage::*UnitEquipmentFireDamageMemberFn )() const;
    UnitEquipmentFireDamageMemberFn equipmentData[nEquipmentStates] =
    {
        &UnitEquipmentFireDamage::available,
        &UnitEquipmentFireDamage::unavailable,
        &UnitEquipmentFireDamage::repairable,
    };
    const char* equipmentStates[nEquipmentStates] =
    {
        "available",
        "unavailable",
        "repairable",
    };

    int ReadMask( xml::xistream& xis )
    {
        if( !xis.has_attribute( "states" ) )
            return 0xFFFF;
        FilterHelper< std::string > states( xis, "states" );
        int result = 0;
        for( unsigned i = 0; i < nEquipmentStates; ++i )
            if( states.IsAllowed( equipmentStates[i] ) )
                result |= ( 1 << i );
        return result;
    }

    bool ReadFireTypeMask( xml::xistream& xis, std::string firetype )
    {
        if( !xis.has_attribute( "fire-types" ) )
            return true;
        FilterHelper< std::string > equipments( xis, "fire-types" );
        bool result = false;
        if( equipments.IsAllowed( firetype ) )
            result = true;
        return result;
    }

    bool ReadFratricideMask( xml::xistream& xis )
    {
        if( !xis.has_attribute( "fratricide" ) )
            return false;
        FilterHelper< std::string > equipments( xis, "fratricide" );
        bool result = false;
        if( equipments.IsAllowed( "true" ) )
            result = true;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: FireComponentDamages constructor
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
FireComponentDamages::FireComponentDamages( xml::xistream& xis )
    : componentFilter_( xis, "components" )
    , stateMask_( ReadMask( xis ) )
    , directFireMask_( ReadFireTypeMask( xis , "direct" ) )
    , indirectFireMask_( ReadFireTypeMask( xis , "indirect" ) )
    , fratricideMask_( ReadFratricideMask( xis ) )
    , partyFilter_( xis, "party" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireComponentDamages::Extract
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
int FireComponentDamages::Extract( const sword::SimToClient& wrapper ) const
{
    const UnitDamagedByUnitFire& damages = wrapper.message().unit_damaged_by_unit_fire();
    if( fratricideMask_ && !damages.fratricide() )
        return 0;
    if( ( directFireMask_ && !damages.direct_fire() ) ||
        !directFireMask_ && damages.direct_fire() )
        return 0;
    if( !partyFilter_.IsAllowed( damages.party().id() ) )
        return 0;
    int result = 0;
    for( int e = 0; e < damages.equipments().elem_size(); ++e )
    {
        const sword::UnitEquipmentFireDamage& damage = damages.equipments().elem( e );
        if( componentFilter_.IsAllowed( damage.equipement().id() ) )
            for( unsigned int i = 0; i < nEquipmentStates; ++i )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    result += ( damage.*equipmentData[ i ] )();
    }
    return result;
}
