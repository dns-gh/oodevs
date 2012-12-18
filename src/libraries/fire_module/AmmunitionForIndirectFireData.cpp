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
AmmunitionForIndirectFireData::AmmunitionForIndirectFireData( const wrapper::View& firer, const std::string& type, const MT_Vector2D* target )
    : firer_        ( firer )
    , type_         ( type )
    , target_       ( target )
    , pCategory_    ( 0 )
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
void AmmunitionForIndirectFireData::ApplyOnWeapon( ModuleFacade& module, const wrapper::View& model, const wrapper::View& component, const wrapper::View& weapon )
{
    const Weapon w( module, model, weapon );
    if( ! w.CanIndirectFire( firer_, component, type_, target_ ) )
        return;
    const DotationCategory& category = w.GetDotation();
    const double value = category.GetValue( firer_ );
    if( ! pCategory_ || dotationValue_ < value )
    {
        pCategory_ = &category;
        dotationValue_ = value;
    }
}

// -----------------------------------------------------------------------------
// Name: AmmunitionForIndirectFireData::GetResult
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
int AmmunitionForIndirectFireData::GetResult() const
{
    return pCategory_ ? pCategory_->GetIdentifier() : -1;
}
