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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentModel.cpp $
// $Author: Age $
// $Modtime: 9/05/05 14:56 $
// $Revision: 14 $
// $Workfile: MOS_AgentModel.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentModel.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentModel.inl"
#endif

#include "MT_Tools/MT_ScipioException.h"
#include "MOS_MainWindow.h"
#include "MOS_App.h"
#include "MOS_Options.h"
#include "MOS_Tools.h"


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel constructor
/** @param  strName 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
MOS_AgentModel::MOS_AgentModel( bool bAutomataModel, const std::string& strName )
: bAutomataModel_ ( bAutomataModel )
, strName_        ( strName )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel destructor
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
MOS_AgentModel::~MOS_AgentModel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::Initialize
/** @param  input 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
void MOS_AgentModel::Initialize( MOS_InputArchive& input )
{
    if( !input.BeginList( "Missions", MOS_InputArchive::eNothing ) )
        return;

    while( input.NextListElement() )
    {
        if( !input.Section( "Mission", MOS_InputArchive::eNothing ) )
            break;
        std::string strMission;
        input.ReadAttribute( "nom", strMission );
        uint nMissionID;
        if( bAutomataModel_ )
            nMissionID = ENT_Tr::ConvertToAutomataMission( strMission );
        else
            nMissionID = ENT_Tr::ConvertToUnitMission( strMission );
        if( nMissionID == (uint)-1 )
            throw MT_ScipioException( "DEC_Model::InitializeMissions", __FILE__, __LINE__, "" );

        vAvailableMissions_.push_back( nMissionID );
        ReadFragOrders( input );
        input.EndSection(); // Mission
    }
    input.EndList(); // Missions    
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::ReadFragOrders
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
void MOS_AgentModel::ReadFragOrders( MOS_InputArchive& input )
{
    if( !input.BeginList( "OrdresConduite", MOS_InputArchive::eNothing ) )
        return;
    while( input.NextListElement() )
    {
        if( !input.Section( "OrdreConduite", MOS_InputArchive::eNothing ) )
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
