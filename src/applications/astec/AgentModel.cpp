// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-22 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentModel.cpp $
// $Author: Age $
// $Modtime: 9/05/05 14:56 $
// $Revision: 14 $
// $Workfile: AgentModel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentModel.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MainWindow.h"
#include "App.h"
#include "Options.h"
#include "Tools.h"


// -----------------------------------------------------------------------------
// Name: AgentModel constructor
/** @param  strName 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
AgentModel::AgentModel( bool bAutomataModel, const std::string& strName )
: bAutomataModel_ ( bAutomataModel )
, strName_        ( strName )
{
}


// -----------------------------------------------------------------------------
// Name: AgentModel destructor
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
AgentModel::~AgentModel()
{
}


// -----------------------------------------------------------------------------
// Name: AgentModel::Initialize
/** @param  input 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
void AgentModel::Initialize( InputArchive& input, bool popu )
{
	if( !input.BeginList( "Missions", InputArchive::eNothing ) )
        return;

    while( input.NextListElement() )
    {
        if( !input.Section( "Mission", InputArchive::eNothing ) )
            break;
        std::string strMission;
        input.ReadAttribute( "nom", strMission );
        uint nMissionID;
		if ( popu )
			nMissionID = ENT_Tr::ConvertToPopulationMission( strMission );
		else
		{
			if( bAutomataModel_ )
				nMissionID = ENT_Tr::ConvertToAutomataMission( strMission );
			else
				nMissionID = ENT_Tr::ConvertToUnitMission( strMission );
		}
        if( nMissionID == (uint)-1 )
            throw MT_ScipioException( "DEC_Model::InitializeMissions", __FILE__, __LINE__, "" );

        vAvailableMissions_.push_back( nMissionID );
        ReadFragOrders( input );
        input.EndSection(); // Mission
    }
    input.EndList(); // Missions    
}

// -----------------------------------------------------------------------------
// Name: AgentModel::ReadFragOrders
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
void AgentModel::ReadFragOrders( InputArchive& input )
{
    if( !input.BeginList( "OrdresConduite", InputArchive::eNothing ) )
        return;
    while( input.NextListElement() )
    {
        if( !input.Section( "OrdreConduite", InputArchive::eNothing ) )
            break;
        std::string strFragOrderName;
        input.ReadAttribute( "nom", strFragOrderName );

        uint nFragOrderID = ENT_Tr::ConvertToFragOrder( strFragOrderName );
        if( nFragOrderID != (uint)( -1 ) )
            fragmentaryOrders_.insert( nFragOrderID );

        input.EndSection(); // OrdreConduite
    };
    input.EndList(); // OrdresConduite
}
