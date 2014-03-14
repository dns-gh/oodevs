// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC constructor
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
MIL_AgentType_ABC::MIL_AgentType_ABC( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : nID_         ( 0 )
    , strName_     ( strName )
    , strType_     ( strType )
    , pNatureLevel_( 0 )
    , pNatureAtlas_( 0 )
    , isTerrorist_ ( false )
    , isMilitia_   ( false )
    , isRefugee_   ( false )
{
    xis >> xml::attribute( "id", nID_ );
    InitializeNature( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC constructor
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
MIL_AgentType_ABC::MIL_AgentType_ABC()
    : nID_         ( 0 )
    , pNatureLevel_( 0 )
    , pNatureAtlas_( 0 )
    , isTerrorist_ ( false )
    , isMilitia_   ( false )
    , isRefugee_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC destructor
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
MIL_AgentType_ABC::~MIL_AgentType_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::InitializeNature
// Created: NLD 2003-05-12
// Modified: JVT 2004-12-09
//-----------------------------------------------------------------------------
void MIL_AgentType_ABC::InitializeNature( xml::xistream& xis )
{
    std::string strBuf;
    xis >> xml::start( "nature" )
        >> xml::attribute( "level", strBuf );
    pNatureLevel_ = PHY_NatureLevel::Find( strBuf );
    if( !pNatureLevel_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown nature level type" );
    xis >> xml::attribute( "atlas-nature", strBuf );
    pNatureAtlas_ = PHY_NatureAtlas::Find( strBuf );
    if( !pNatureAtlas_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown atlas qualificatif" );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetID
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
unsigned int MIL_AgentType_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetNatureLevel
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
const PHY_NatureLevel& MIL_AgentType_ABC::GetNatureLevel() const
{
    assert( pNatureLevel_ );
    return *pNatureLevel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetNatureAtlas
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
const PHY_NatureAtlas& MIL_AgentType_ABC::GetNatureAtlas() const
{
    assert( pNatureAtlas_ );
    return *pNatureAtlas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetName
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
const std::string& MIL_AgentType_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetName
// Created: LMT 2010-09-29
// -----------------------------------------------------------------------------
const std::string& MIL_AgentType_ABC::GetMilPionType() const
{
    return strType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::IsMilitia
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool MIL_AgentType_ABC::IsMilitia() const
{
    return isMilitia_;
}

void MIL_AgentType_ABC::SetMilitia( bool isMilitia )
{
    isMilitia_ = isMilitia;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::IsTerrorist
// Created: NLD 2006-02-23
// -----------------------------------------------------------------------------
bool MIL_AgentType_ABC::IsTerrorist() const
{
    return isTerrorist_;
}

void MIL_AgentType_ABC::SetTerrorist( bool isTerrorist )
{
    isTerrorist_ = isTerrorist;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool MIL_AgentType_ABC::IsRefugee() const
{
    return isRefugee_;
}

void MIL_AgentType_ABC::SetRefugee( bool isRefugee )
{
    isRefugee_ = isRefugee;
}
