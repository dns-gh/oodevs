// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 16:42 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_HumanFactors.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RolePion_HumanFactors.h"

#include "Network/NET_ASN_Messages.h"

#include "xeumeuleu/xml.h"

using namespace xml;

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_HumanFactors, "PHY_RolePion_HumanFactors" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::PHY_RolePion_HumanFactors( MT_RoleContainer& role )
    : PHY_RoleInterface_HumanFactors( role )
    , bHasChanged_                  ( true )
    , pMorale_                      ( &PHY_Morale    ::bon_     )
    , pExperience_                  ( &PHY_Experience::veteran_ )
    , pTiredness_                   ( &PHY_Tiredness ::normal_  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::PHY_RolePion_HumanFactors()
    : PHY_RoleInterface_HumanFactors()
    , bHasChanged_                  ( true )
    , pMorale_                      ( 0 )
    , pExperience_                  ( 0 )
    , pTiredness_                   ( 0 )

{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::~PHY_RolePion_HumanFactors()
{
}


// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( !pMorale_ );
    assert( !pExperience_ );
    assert( !pTiredness_ );
    
    file >> boost::serialization::base_object< PHY_RoleInterface_HumanFactors >( *this );
    
    uint nID;
    file >> nID;
    pMorale_ = PHY_Morale::Find( nID );
    assert( pMorale_ );
    
    file >> nID;
    pExperience_ = PHY_Experience::Find( nID );
    assert( pExperience_ );
    
    file >> nID;
    pTiredness_ = PHY_Tiredness::Find( nID );
    assert( pTiredness_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pMorale_ );
    assert( pExperience_ );
    assert( pTiredness_ );
    unsigned morale     = pMorale_->GetID(),
             experience = pExperience_->GetID(),
             tiredness  = pTiredness_->GetID();
    file << boost::serialization::base_object< PHY_RoleInterface_HumanFactors >( *this )
         << morale
         << experience
         << tiredness;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadOverloading
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadOverloading( xml::xistream& xis )
{
    xis >> list( "FacteursHumains", *this, &PHY_RolePion_HumanFactors::ReadFacteursHumains );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadFacteursHumains
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadFacteursHumains( xml::xistream& xis )
{

    xis >> list( "Fatigue", *this, &PHY_RolePion_HumanFactors::ReadFatigue )
        >> list( "Moral", *this, &PHY_RolePion_HumanFactors::ReadMoral )
        >> list( "Experience", *this, &PHY_RolePion_HumanFactors::ReadExperience );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadFatigue
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadFatigue( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pTiredness_ = PHY_Tiredness::Find( strTmp );
    if( !pTiredness_ )
        xis.error( "Unknown tiredness type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadMoral
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadMoral( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pMorale_ = PHY_Morale::Find( strTmp );
    if( !pMorale_)
        xis.error( "Unknown morale type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadExperience
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadExperience( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pExperience_ = PHY_Experience::Find( strTmp );
    if( !pExperience_ )
        xis.error( "Unknown experience type" );
}
// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.fatiguePresent    = 1;
    msg().m.moralPresent      = 1;
    msg().m.experiencePresent = 1;

    msg().fatigue    = pTiredness_ ->GetAsnID();
    msg().moral      = pMorale_    ->GetAsnID();
    msg().experience = pExperience_->GetAsnID();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

