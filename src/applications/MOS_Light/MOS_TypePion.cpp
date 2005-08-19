// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_TypePion.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 11:44 $
// $Revision: 1 $
// $Workfile: MOS_TypePion.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_TypePion.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_TypePion constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypePion::MOS_TypePion( const std::string& strName, MT_InputArchive_ABC& archive )
    : strName_( strName )
    , pModel_ ( 0 )
{
    if( !archive.ReadField( "MosID", nID_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    std::string strModel;
    if( !archive.ReadField( "ModeleDecisionnel", strModel ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    pModel_ = MOS_App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion destructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypePion::~MOS_TypePion()
{

}
