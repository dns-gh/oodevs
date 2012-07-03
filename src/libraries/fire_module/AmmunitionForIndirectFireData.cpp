// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "AmmunitionForIndirectFireData.h"
#include "Weapon.h"
#include "DotationCategory.h"
#include <wrapper/Hook.h>

using namespace sword;
using namespace sword::fire;

// -----------------------------------------------------------------------------
// Name: AmmunitionForIndirectFireData constructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
AmmunitionForIndirectFireData::AmmunitionForIndirectFireData( const wrapper::View& firer, const std::string& type, const MT_Vector2D& target )
    : firer_        ( firer )
    , type_         ( type )
    , target_       ( target )
    , pAmmunition_  ( 0 )
    , dotationValue_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AmmunitionForIndirectFireData destructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
AmmunitionForIndirectFireData::~AmmunitionForIndirectFireData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AmmunitionForIndirectFireData::ApplyOnWeapon
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void AmmunitionForIndirectFireData::ApplyOnWeapon( const wrapper::View& model, const wrapper::View& component, const wrapper::View& weapon )
{
    const wrapper::View& movement = firer_[ "movement" ];
    const MT_Vector2D position( movement[ "position/x" ], movement[ "position/y" ] );
    const double range = position.Distance( target_ );
    const Weapon w( model, weapon );
    if( ! w.CanIndirectFire( firer_, component, range, type_ ) )
        return;
    const DotationCategory& ammunition = w.GetDotation();
    const double value = ammunition.GetValue( firer_ );
    if( ! pAmmunition_ || dotationValue_ < value )
    {
        pAmmunition_ = &ammunition;
        dotationValue_ = value;
    }
}

// -----------------------------------------------------------------------------
// Name: AmmunitionForIndirectFireData::GetResult
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
const char* AmmunitionForIndirectFireData::GetResult() const
{
    return pAmmunition_ ? pAmmunition_->GetName().c_str() : 0;
}
