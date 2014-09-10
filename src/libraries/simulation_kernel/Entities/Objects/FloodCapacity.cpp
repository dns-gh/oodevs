// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodCapacity.h"
#include "FloodAttribute.h"
#include "MIL_FloodEffectManipulator.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/WoundEffectsHandler_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"

BOOST_CLASS_EXPORT_IMPLEMENT( FloodCapacity )

// -----------------------------------------------------------------------------
// Name: FloodCapacity constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodCapacity::FloodCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodCapacity::FloodCapacity( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "injuries" )
        >> xml::list( "injury", *this, &FloodCapacity::ReadInjury )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity constructor
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodCapacity::FloodCapacity( const FloodCapacity& from )
    : injuries_( from.injuries_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity destructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodCapacity::~FloodCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::load
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
void FloodCapacity::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file >> boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    file >> injuries_;
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::save
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
void FloodCapacity::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file << boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    file << injuries_;
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::Instanciate
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    FloodCapacity* capacity = new FloodCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::Finalize
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
void FloodCapacity::Finalize( MIL_Object_ABC& object )
{
    const FloodAttribute& flood = object.GetAttribute< FloodAttribute >();
    TER_Agent_ABC::T_AgentPtrVector agentsInsideObject;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( flood.GetLocalisation(), agentsInsideObject );
    for( auto itAgent = agentsInsideObject.begin(); itAgent != agentsInsideObject.end(); ++itAgent )
        static_cast< PHY_RoleInterface_Location& >( **itAgent ).NotifyTerrainPutInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::Register
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::ProcessAgentInside
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
void FloodCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    E_CrossingHeight crossingHeight = agent.GetType().GetUnitType().GetCrossingHeight();
    if( crossingHeight != eCrossingHeightAlways )
    {
        bool isHurt = false;
        const MT_Vector2D& position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
        geometry::Point2f point( static_cast< float>( position.rX_ ), static_cast< float >( position.rY_ ) );
        const FloodAttribute& flood = object.GetAttribute< FloodAttribute >();
        const auto& deepAreas = flood.GetDeepAreas();
        for( auto it = deepAreas.begin(); it != deepAreas.end(); ++it )
            if( it->BoundingBox().IsInside( point ) && it->IsInside( point ) )
            {
                isHurt = true;
                break;
            }

        if( !isHurt && crossingHeight == eCrossingHeightNever )
        {
            const auto& lowAreas = flood.GetLowAreas();
            for( auto it = lowAreas.begin(); it != lowAreas.end(); ++it )
                if( it->BoundingBox().IsInside( point ) && it->IsInside( point ) )
                {
                    isHurt = true;
                    break;
                }
        }
        if( isHurt )
        {
            MIL_FloodEffectManipulator manipulator( injuries_ );
            agent.Apply( &WoundEffectsHandler_ABC::ApplyEffect, manipulator );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::ReadInjury
// Created: JSR 2011-01-10
// -----------------------------------------------------------------------------
void FloodCapacity::ReadInjury( xml::xistream& xis )
{
    double percentage;
    std::string type;
    xis >> xml::attribute( "type", type )
        >> xml::attribute( "percentage", percentage );
    const PHY_HumanWound* pType = PHY_HumanWound::Find( type );
    if( !pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown injury type " + type );
    injuries_.insert( std::make_pair( pType, percentage ) );
}
