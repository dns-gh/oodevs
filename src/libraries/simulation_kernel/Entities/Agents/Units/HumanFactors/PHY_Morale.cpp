// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/HumanFactors/PHY_Morale.cpp $
// $Author: Nld $
// $Modtime: 1/12/04 10:36 $
// $Revision: 2 $
// $Workfile: PHY_Morale.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Morale.h"
#include "protocol/protocol.h"

PHY_Morale::T_MoraleMap PHY_Morale::morales_;

const PHY_Morale PHY_Morale::mauvais_  ( "Mauvais"  , eMauvais  , Common::EnumUnitMorale::mauvais  , 0.5  );
const PHY_Morale PHY_Morale::moyen_    ( "Moyen"    , eMoyen    , Common::EnumUnitMorale::moyen    , 0.75 );
const PHY_Morale PHY_Morale::bon_      ( "Bon"      , eBon      , Common::EnumUnitMorale::bon      , 1    );
const PHY_Morale PHY_Morale::fanatique_( "Fanatique", eFanatique, Common::EnumUnitMorale::fanatique, 1.25 );

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Morale::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing morales" );
    morales_[ mauvais_  .GetName() ] = &mauvais_;
    morales_[ moyen_    .GetName() ] = &moyen_;
    morales_[ bon_      .GetName() ] = &bon_;
    morales_[ fanatique_.GetName() ] = &fanatique_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Morale::Terminate()
{
    morales_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Morale::PHY_Morale( const std::string& strName, E_MoraleType nType, Common::EnumUnitMorale nAsnID, MT_Float rDIAWeight )
    : strName_   ( strName )
    , nType_     ( nType   )
    , nAsnID_    ( nAsnID  )
    , rDIAWeight_( rDIAWeight )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Morale::~PHY_Morale()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Morale* PHY_Morale::Find( Common::EnumUnitMorale nAsnID )
{
    CIT_MoraleMap it = std::find_if( morales_.begin(), morales_.end(), std::compose1( std::bind2nd( std::equal_to< Common::EnumUnitMorale >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Morale::GetAsnID ), std::select2nd< T_MoraleMap::value_type >() ) ) );

    return it == morales_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Morale* PHY_Morale::Find( unsigned int nID )
{
    CIT_MoraleMap it = std::find_if( morales_.begin(), morales_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_Morale::GetID ), std::select2nd< T_MoraleMap::value_type >() ) ) );

    return it == morales_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
const PHY_Morale* PHY_Morale::Find( const std::string& strName )
{
    CIT_MoraleMap it = morales_.find( strName );

    return it == morales_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Morale::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
Common::EnumUnitMorale PHY_Morale::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_Morale::operator==( const PHY_Morale& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_Morale::operator!=( const PHY_Morale& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
unsigned int PHY_Morale::GetID() const
{
    return (unsigned int)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetWeight
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
MT_Float PHY_Morale::GetWeight() const
{
    return rDIAWeight_;
}
