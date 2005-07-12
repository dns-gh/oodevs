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

#include "MOS_Light2_Pch.h"

#include "MOS_TypeAutomate.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypeAutomate::MOS_TypeAutomate( const std::string& strName, MT_InputArchive_ABC& archive )
    : strName_( strName )
    , pModel_ ( 0 )
    , pTypePC_( 0 )
{
    if( !archive.ReadField( "MosID", nID_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !archive.Section( "Automate" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    std::string strModel;
    if( !archive.ReadField( "ModeleDecisionnel", strModel ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    pModel_ = MOS_App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    archive.EndSection(); // Automate

    if( !archive.Section( "PionPC" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    std::string strPCType;
    if( !archive.ReadAttribute( "type", strPCType ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

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
