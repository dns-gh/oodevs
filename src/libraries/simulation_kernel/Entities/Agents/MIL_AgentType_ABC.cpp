// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentType_ABC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:50 $
// $Revision: 8 $
// $Workfile: MIL_AgentType_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentType_ABC.h"

#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC constructor
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
MIL_AgentType_ABC::MIL_AgentType_ABC( const std::string& strName, xml::xistream& xis )
    : nID_         ( 0 )
    , strName_     ( strName )
    , pNatureLevel_( 0 )
    , pNatureAtlas_( 0 )
{
    xis >> attribute( "id", nID_ );
    InitializeNature( xis );
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
    xis >> start( "nature" )
        >> attribute( "level", strBuf );
    pNatureLevel_ = PHY_NatureLevel::Find( strBuf );
    if ( !pNatureLevel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown nature level type" ); // $$$$ ABL 2007-07-23: error context

    xis >> attribute( "atlas-nature", strBuf );
    pNatureAtlas_ = PHY_NatureAtlas::Find( strBuf );
    if ( !pNatureAtlas_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown atlas qualificatif" ); // $$$$ ABL 2007-07-23: error context
    xis >> end();
}
