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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Tools.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( AgentFactory )

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( MIL_IDManager& idManager, MissionController_ABC& missionController )
    : idManager_          ( idManager )
    , missionController_  ( missionController )
    , algorithmsFactories_( new AlgorithmsFactories() )
{
    // NOTHING
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
    MIL_AgentPion* pPion = type.InstanciatePion( automate, *algorithmsFactories_, xis );
    type.RegisterRoles( *pPion );
    pPion->Register( missionController_ );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    return Create( type, automate, vPosition, type.GetName() );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MMC 2011-05-27
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& /*vPosition*/, const std::string& name )
{
    MIL_AgentPion* pPion = type.InstanciatePion( automate, *algorithmsFactories_, name );
    type.RegisterRoles( *pPion );
    pPion->Register( missionController_ );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AgentFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < AgentFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AgentFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < AgentFactory_ABC >( *this );
}
