// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypeAutomate.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypeAutomate.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "TypeAutomate.h"
#include "App.h"
#include "AgentManager.h"

// -----------------------------------------------------------------------------
// Name: TypeAutomate constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
TypeAutomate::TypeAutomate( const std::string& strName, InputArchive& archive )
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
    pModel_ = App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    archive.EndSection(); // Automate

    archive.Section( "PionPC" );
    std::string strPCType;
    archive.ReadAttribute( "type", strPCType );
    pTypePC_ = App::GetApp().GetAgentManager().FindTypePion( strPCType );
    if( !pTypePC_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    archive.EndSection(); // PionPC

}

// -----------------------------------------------------------------------------
// Name: TypeAutomate destructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
TypeAutomate::~TypeAutomate()
{

}
