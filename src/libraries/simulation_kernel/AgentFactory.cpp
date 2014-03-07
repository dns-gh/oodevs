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
#include "MissionController_ABC.h"
#include "AlgorithmsFactories.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Checkpoints/SerializationTools.h"
#include "Tools/MIL_IDManager.h"
#include <boost/serialization/map.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( AgentFactory )

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: MCO 2013-02-22
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( MIL_IDManager& idManager, MissionController_ABC& missionController, std::auto_ptr< AlgorithmsFactories > algorithmsFactories )
    : idManager_          ( idManager )
    , missionController_  ( missionController )
    , algorithmsFactories_( algorithmsFactories )
{
    if( !algorithmsFactories_.get() )
        algorithmsFactories_.reset( new AlgorithmsFactories() );
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
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext )
{
    MIL_AgentPion* pPion = type.InstanciatePion( *algorithmsFactories_, missionController_, automate, xis );
    type.RegisterRoles( *pPion, ext );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MMC 2011-05-27
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& /*vPosition*/, const std::string* name, sword::RoleExtender_ABC* ext )
{
    const std::string unitName = name ? *name : type.GetName();
    MIL_AgentPion* pPion = type.InstanciatePion( *algorithmsFactories_,
            missionController_, automate, unitName );
    type.RegisterRoles( *pPion, ext );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AgentFactory::load( MIL_CheckPointInArchive&, const unsigned int )
{
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AgentFactory::save( MIL_CheckPointOutArchive&, const unsigned int ) const
{
}
