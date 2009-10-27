// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AgentFactory.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Automates/MIL_Automate.h"

#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Tools.h"

#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/Decision/DEC_DataBase.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( AgentFactory, "AutomateFactory" )

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( MIL_IDManager& idManager, DEC_DataBase& database )
    : idManager_( idManager ) 
    , algorithmsFactories_( new AlgorithmsFactories() )
    , database_( database )
{

}

// -----------------------------------------------------------------------------
// Name: AgentFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
AgentFactory::~AgentFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    uint        id;
    xis >> xml::attribute( "id", id );//TODO refactor IDManager to reserve ID

    MIL_AgentPion* pPion = type.InstanciatePion( id, automate, *algorithmsFactories_, xis );//@TODO REPLACE WHEN PIONLOG WILL BE DELETED
    type.RegisterRoles( *pPion , database_ );
    
    std::string strPosition;
    xis >> xml::attribute( "position", strPosition );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( strPosition, vPosTmp );

    Initialize( *pPion, automate, vPosTmp );

    pPion->ReadOverloading( xis );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    MIL_AgentPion* pPion = type.InstanciatePion( idManager_.GetFreeId(), automate, *algorithmsFactories_ );
    type.RegisterRoles( *pPion, database_ );

    Initialize( *pPion, automate, vPosition );
    return pPion;
}

void AgentFactory::Initialize( MIL_AgentPion& pion, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    automate.RegisterPion( pion );

    pion.GetRole< PHY_RoleInterface_Location >().Move( vPosition, MT_Vector2D( 0., 1. ), 0. );
}

