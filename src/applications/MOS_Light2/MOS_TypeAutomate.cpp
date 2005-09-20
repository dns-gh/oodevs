// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypeAutomate.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypeAutomate.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_TypeAutomate.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypeAutomate::MOS_TypeAutomate( const std::string& strName, MOS_InputArchive& archive )
    : strName_( strName )
	, strType_( std::string("") )
    , pModel_ ( 0 )
    , pTypePC_( 0 )
{
	
	archive.ReadAttribute( "type", strType_ );
	archive.ReadField( "MosID", nID_ );
    archive.Section( "Automate" );
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );
    pModel_ = MOS_App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    archive.EndSection(); // Automate

    archive.Section( "PionPC" );
    std::string strPCType;
    archive.ReadAttribute( "type", strPCType );
    pTypePC_ = MOS_App::GetApp().GetAgentManager().FindTypePion( strPCType );
    if( !pTypePC_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    archive.EndSection(); // PionPC

}

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate destructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypeAutomate::~MOS_TypeAutomate()
{

}
