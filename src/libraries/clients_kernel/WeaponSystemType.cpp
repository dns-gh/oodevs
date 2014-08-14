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
#include "VolumeType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeaponSystemType constructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
WeaponSystemType::WeaponSystemType( xml::xistream& xis, const tools::Resolver_ABC< VolumeType >& volumes )
    : launcher_        ( xis.attribute< std::string >( "launcher" ) )
    , ammunition_      ( xis.attribute< std::string >( "munition" ) )
    , maxIndirectRange_( 0 )
    , minIndirectRange_( std::numeric_limits< unsigned int >::max() )
    , maxDirectRange_  ( 0 )
    , minDirectRange_  ( std::numeric_limits< unsigned int >::max() )
    , volumes_         ( volumes )
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
    xis >> xml::optional >> xml::start( "direct-fire" )
            >> xml::list( "hit-probabilities", *this, &WeaponSystemType::ReadDirectFireHitProbabilities )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadDirectFireHitProbabilities
// Created: SBO 2008-08-14
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadDirectFireHitProbabilities( xml::xistream& xis )
{
    std::string targetType;
    xis >> xml::attribute( "target", targetType );
    MT_InterpolatedFunction* phFunction = 0;
    tools::Iterator< const kernel::VolumeType& > it = volumes_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::VolumeType& volume = it.NextElement();
        if( volume.GetName() == targetType )
        {
            phFunction = &phs_[ volume.GetId() ];
            break;
        }
    }
    xis >> xml::list( "hit-probability", *this, &WeaponSystemType::ReadDirectFireHitProbability, phFunction );
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadDirectFireHitProbability
// Created: SBO 2008-08-14
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadDirectFireHitProbability( xml::xistream& xis, MT_InterpolatedFunction* phFunction )
{
    unsigned int distance = 0;
    float percentage = 0;
    xis >> xml::attribute( "distance", distance )
        >> xml::attribute( "percentage", percentage );
    if( percentage > 0 )
        minDirectRange_ = std::min( minDirectRange_, distance );
    maxDirectRange_ = std::max( maxDirectRange_, distance );
    if( phFunction )
        phFunction->AddNewPoint( distance, percentage );
}

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::ReadIndirectFire
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void WeaponSystemType::ReadIndirectFire( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "indirect-fire" )
            >> xml::attribute( "max-range", maxIndirectRange_ )
            >> xml::attribute( "min-range", minIndirectRange_ )
        >> xml::end;
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

// -----------------------------------------------------------------------------
// Name: WeaponSystemType::GetEfficientRange
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
unsigned int WeaponSystemType::GetEfficientRange( unsigned int volumeId, double ph ) const
{
    T_HitProbabilities::const_iterator it = phs_.find( volumeId );
    if( it != phs_.end() )
        return unsigned int( it->second.GetMaxYForX( ph ) );
    return 0;
}

bool WeaponSystemType::IsIndirect() const
{
    return maxIndirectRange_ > 0;
}
