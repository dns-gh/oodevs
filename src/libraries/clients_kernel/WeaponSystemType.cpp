// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "WeaponSystemType.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeaponSystemType constructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
WeaponSystemType::WeaponSystemType( xml::xistream& xis )
    : launcher_        ( xml::attribute< std::string >( xis, "launcher" ) )
    , ammunition_      ( xml::attribute< std::string >( xis, "munition" ) )
    , maxIndirectRange_( 0 )
    , minIndirectRange_( std::numeric_limits< unsigned int >::max() )
    , maxDirectRange_  ( 0 )
    , minDirectRange_  ( std::numeric_limits< unsigned int >::max() )
{
    ReadDirectFire( xis );
    ReadIndirectFire( xis );
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType destructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
WeaponSystemType::~WeaponSystemType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadDirectFire
// Created: SBO 2008-08-14
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadDirectFire( xml::xistream& xis )
{
    xis >> xml::optional()
        >> xml::start( "direct-fire" )
            >> xml::list( "hit-probabilities", *this, &WeaponSystemType::ReadDirectFireHitProbabilities )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadDirectFireHitProbabilities
// Created: SBO 2008-08-14
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadDirectFireHitProbabilities( xml::xistream& xis )
{
    xis >> xml::list( "hit-probability", *this, &WeaponSystemType::ReadDirectFireHitProbability );
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadDirectFireHitProbability
// Created: SBO 2008-08-14
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadDirectFireHitProbability( xml::xistream& xis )
{
    unsigned int distance = 0;
    float percentage = 0;
    xis >> xml::attribute( "distance", distance )
        >> xml::attribute( "percentage", percentage );
    if( percentage > 0 )
    {
        minDirectRange_ = std::min( minDirectRange_, distance );
        maxDirectRange_ = std::max( maxDirectRange_, distance );
    }
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadIndirectFire
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadIndirectFire( xml::xistream& xis )
{
    xis >> xml::optional()
        >> xml::start( "indirect-fire" )
            >> xml::attribute( "max-range", maxIndirectRange_ )
            >> xml::attribute( "min-range", minIndirectRange_ )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::GetId
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
std::string WeaponSystemType::GetId() const
{
    return launcher_ + ammunition_;
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::GetMaxRange
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
unsigned int WeaponSystemType::GetMaxRange() const
{
    return std::max( maxIndirectRange_, maxDirectRange_ );
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::GetMinRange
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
unsigned int WeaponSystemType::GetMinRange() const
{
    return std::min( minIndirectRange_, minDirectRange_ );
}
