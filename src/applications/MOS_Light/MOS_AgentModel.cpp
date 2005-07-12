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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentModel.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_AgentModel.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_AgentModel.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentModel.inl"
#endif

#include "MT_Tools/MT_ScipioException.h"
#include "MOS_Tools.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentModel constructor
/** @param  strName 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
MOS_AgentModel::MOS_AgentModel( const std::string& strName )
: strName_( strName )
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
void MOS_AgentModel::Initialize( MT_InputArchive_ABC& input )
{
    InitializeMission( input );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::InitializeMission
/** @param  input 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
void MOS_AgentModel::InitializeMission( MT_InputArchive_ABC& input )
{
    if( !input.BeginList( "Missions" ) )
    {
        input.ClearErrors();
        return;
    }

    while( input.NextListElement() )
    {
        if( !input.Section( "Mission" ) )
        {
            input.ClearErrors();
            break;
        }
        std::string strMission;
        if( !input.ReadAttribute( "nom", strMission ) )
            throw MT_ScipioException( "DEC_Model::InitializeMissions", __FILE__, __LINE__, "" );

        E_MissionID nMissionID = MOS_Tools::ConvertMissionType( strMission );
        if( nMissionID == (E_MissionID)-1 )
            throw MT_ScipioException( "DEC_Model::InitializeMissions", __FILE__, __LINE__, "" );

        vAvailableMissions_.push_back( nMissionID );

        input.EndSection(); // Mission
    }
    input.EndList(); // Missions    
}
