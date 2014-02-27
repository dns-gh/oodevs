// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActiveProtection.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Orders/MIL_Report.h"
#include "MIL_Random.h"

namespace
{
    static std::map< std::string, boost::shared_ptr< PHY_ActiveProtection > > protections;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection constructor
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
PHY_ActiveProtection::PHY_ActiveProtection( xml::xistream& xis )
    : coefficient_( 0 )
    , usage_    ( 0. )
    , hardKill_ ( false )
    , pDotation_( 0 )
{
    std::string strDotationName;
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "coefficient", coefficient_ )
        >> xml::optional >> xml::start( "resource" )
            >> xml::attribute( "name", strDotationName )
            >> xml::attribute( "usage", usage_ )
        >> xml::end
        >> xml::optional >> xml::attribute( "hard-kill", hardKill_ )
        >> xml::list( "weapon", *this, &PHY_ActiveProtection::ReadWeapon );
    if( !strDotationName.empty() )
        pDotation_ = PHY_DotationType::FindDotationCategory( strDotationName );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection destructor
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
PHY_ActiveProtection::~PHY_ActiveProtection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::ReadWeapon
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ActiveProtection::ReadWeapon( xml::xistream& xis )
{
    std::string name;
    double value = 0;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "coefficient", value );
    weapons_[ PHY_DotationType::FindDotationCategory( name ) ] = value;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::Initialize
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ActiveProtection::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "protections" )
            >> xml::list( "protection", &PHY_ActiveProtection::Create )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::Create
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ActiveProtection::Create( xml::xistream& xis )
{
    boost::shared_ptr< PHY_ActiveProtection > pProtection( new PHY_ActiveProtection( xis ) );
    protections[ pProtection->name_ ] = pProtection;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::Terminate
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ActiveProtection::Terminate()
{
    protections.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::Find
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
PHY_ActiveProtection* PHY_ActiveProtection::Find( const std::string& name )
{
    auto it = protections.find( name );
    if( it == protections.end() )
        return 0;
    return it->second.get();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::UseAmmunition
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
void PHY_ActiveProtection::UseAmmunition( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    if( pDotation_ && GetCoefficient( category ) != 1. )
        pion.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( *pDotation_, usage_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::GetPHModifier
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
double PHY_ActiveProtection::GetPHModifier( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    return HasAmmo( pion ) ? GetCoefficient( category ) : 1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::CounterIndirectFire
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
bool PHY_ActiveProtection::CounterIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    bool result = HasAmmo( pion ) ? ( !hardKill_ && MIL_Random::rand_oi( MIL_Random::eFire ) > GetCoefficient( category ) ) : false;
    if( result )
        MIL_Report::PostEvent( pion, report::eRC_ProtectionActiveReussie );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::DestroyIndirectFire
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
bool PHY_ActiveProtection::DestroyIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    bool result = HasAmmo( pion ) ? ( hardKill_ && MIL_Random::rand_oi( MIL_Random::eFire ) > GetCoefficient( category ) ) : false;
    if( result )
        MIL_Report::PostEvent( pion, report::eRC_ProtectionActiveHardKillReussie );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::GetCoefficient
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
double PHY_ActiveProtection::GetCoefficient( const PHY_DotationCategory& category ) const
{
    auto it = weapons_.find( &category );
    if( it == weapons_.end() )
        return coefficient_;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActiveProtection::HasAmmo
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
bool PHY_ActiveProtection::HasAmmo( MIL_Agent_ABC& pion ) const
{
    return pion.GetRole< dotation::PHY_RoleInterface_Dotations >().GetDotationNumber( *pDotation_ ) >= usage_;
}
