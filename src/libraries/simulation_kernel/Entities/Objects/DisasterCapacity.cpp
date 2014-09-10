// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DisasterCapacity.h"
#include "Object.h"
#include "DisasterAttribute.h"
#include "MIL_DisasterType.h"
#include "MIL_Time_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Humans/PHY_NbcSuit.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DisasterCapacity )

// -----------------------------------------------------------------------------
// Name: DisasterCapacity constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterCapacity::DisasterCapacity( xml::xistream& xis )
    : type_( xis.attribute< std::string >( "model" ) )
{
    disasterType_ = MIL_DisasterType::Find( type_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterCapacity::DisasterCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterCapacity::DisasterCapacity( const DisasterCapacity& from )
    : type_( from.type_ )
{
    disasterType_ = MIL_DisasterType::Find( type_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity destructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterCapacity::~DisasterCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::load
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void DisasterCapacity::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    archive >> type_;
    disasterType_ = MIL_DisasterType::Find( type_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::save
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void DisasterCapacity::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    archive << type_;
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::Register
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::Instanciate
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    DisasterCapacity* capacity = new DisasterCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::Update
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterCapacity::Update( MIL_Object_ABC& object, unsigned int time )
{
    DisasterAttribute* disaster = object.RetrieveAttribute< DisasterAttribute >();
    if( disaster )
        disaster->UpdateLocalisation( object, time );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::GetDose
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
float DisasterCapacity::GetDose( const MIL_Object_ABC& object, const MT_Vector2D& position ) const
{
    const DisasterAttribute* disaster = object.RetrieveAttribute< DisasterAttribute >();
    if( !disaster )
        return 0.f;
    return disaster->GetDose( position );
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::ProcessAgentInside
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void DisasterCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
     if( !agent.IsDead() )
     {
         float dose = GetDose( object, agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
         if( dose > 0.f )
         {
             int step = MIL_Time_ABC::GetTime().GetTickDuration();
             const float protection = disasterType_->GetProtectionCoefficient(
                 agent.GetType().GetUnitType().GetNbcSuit() );
             dose = std::pow( dose * protection, disasterType_->GetToxicityExponent() ) * step;
             if( dose > 0.f )
                 agent.GetRole< nbc::PHY_RoleInterface_NBC >().Afflict( dose, *disasterType_ );
         }
     }
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::GetDisasterType
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const MIL_DisasterType& DisasterCapacity::GetDisasterType() const
{
    return *disasterType_;
}

// -----------------------------------------------------------------------------
// Name: DisasterCapacity::CanBeSeen
// Created: LGY 2012-12-07
// -----------------------------------------------------------------------------
bool DisasterCapacity::CanBeSeen() const
{
    return false;
}
