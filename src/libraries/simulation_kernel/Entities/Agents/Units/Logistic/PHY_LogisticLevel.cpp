// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_LogisticLevel.cpp $
// $Author: Jvt $
// $Modtime: 9/12/04 15:09 $
// $Revision: 1 $
// $Workfile: PHY_LogisticLevel.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_LogisticLevel.h"
#include "protocol/Protocol.h"

PHY_LogisticLevel::T_LogisticLevelMap PHY_LogisticLevel::logisticLevels_;

// =============================================================================
// MANAGER
// =============================================================================

PHY_LogisticLevel PHY_LogisticLevel::none_( "aucun", sword::none );
PHY_LogisticLevel PHY_LogisticLevel::logistic_base_  ( "logistic-base", sword::logistic_base  );

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::Initialize
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
void PHY_LogisticLevel::Initialize()
{
    logisticLevels_[ none_.GetName() ] = &none_;
    logisticLevels_[ logistic_base_  .GetName() ] = &logistic_base_  ;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_LogisticLevel::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel constructor
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
PHY_LogisticLevel::PHY_LogisticLevel( const std::string& strName, int id )
    : strName_ ( strName )
    , id_ ( id )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel destructor
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
PHY_LogisticLevel::~PHY_LogisticLevel()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::operator==
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
bool PHY_LogisticLevel::operator==( const PHY_LogisticLevel& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::operator!=
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
bool PHY_LogisticLevel::operator!=( const PHY_LogisticLevel& rhs ) const
{
    return this != &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::Find
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
const PHY_LogisticLevel* PHY_LogisticLevel::Find( const std::string& strName )
{
    CIT_LogisticLevelMap it = logisticLevels_.find( strName );
    return it == logisticLevels_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::GetName
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
const std::string& PHY_LogisticLevel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogisticLevel::GetID
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
int PHY_LogisticLevel::GetID() const
{
    return id_;
}
