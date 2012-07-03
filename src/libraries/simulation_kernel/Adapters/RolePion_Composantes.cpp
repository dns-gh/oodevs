// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RolePion_Composantes.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include <core/Model.h>
#include <core/Facade.h>
#include <core/Convert.h>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RolePion_Composantes )

using namespace sword;

namespace sword
{
    template< typename Archive >
    void save_construct_data( Archive& archive, const sword::RolePion_Composantes* role, const unsigned int /*version*/ )
    {
        const MIL_Agent_ABC* const pion = &role->pion_;
        const core::Model* const entity = &role->entity_;
        archive << pion << entity;
    }
    template< typename Archive >
    void load_construct_data( Archive& archive, sword::RolePion_Composantes* role, const unsigned int /*version*/ )
    {
        MIL_Agent_ABC* pion;
        core::Model* entity;
        archive >> pion >> entity;
        ::new( role )sword::RolePion_Composantes( *pion, *entity );
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes constructor
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
RolePion_Composantes::RolePion_Composantes( MIL_Agent_ABC& pion, core::Model& entity )
    : PHY_RolePion_Composantes( pion, false ) // $$$$ MCO 2012-05-04: false => don't init in PHY_RolePion_Composantes because NotifyComposanteAdded in this sub-class won't be called
    , pion_  ( pion )
    , entity_( entity )
{
    entity[ "components" ];
    pion.GetType().GetUnitType().InstanciateComposantes( *this );
    DistributeCommanders();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes destructor
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
RolePion_Composantes::~RolePion_Composantes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::serialize
// Created: SLI 2012-03-27
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Composantes::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RolePion_Composantes >( *this );
}

namespace
{
    void AddSensor( core::Model& component, const PHY_Sensor& sensor )
    {
        core::Model& s = component[ "sensors" ].AddElement();
        s[ "height" ] = sensor.GetHeight();
        s[ "type" ] = sensor.GetType().GetName();
    }
    void AddRadar( core::Model& component, const PHY_RadarType& radar )
    {
        core::Model& s = component[ "radars" ].AddElement();
        s[ "type" ] = radar.GetName();
    }
    void AddWeapon( core::Model& component, const PHY_Weapon& weapon )
    {
        core::Model& w = component[ "weapons" ].AddElement();
        w[ "type" ] = weapon.GetType().GetName();
        w[ "fired-ammo" ] = 0;
        w[ "next-time" ] = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::NotifyComposanteAdded
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
void RolePion_Composantes::NotifyComposanteAdded( PHY_ComposantePion& composante )
{
    core::Model& component = entity_[ "components" ].AddElement();
    component[ "component" ].SetUserData( &composante );
    component[ "major" ] = composante.IsMajor();
    component[ "volume" ] = composante.GetType().GetVolume().GetID();
    component[ "score" ] = composante.GetMajorScore();
    component[ "radars" ];
    component[ "sensors" ];
    component[ "type/sensor-rotation-angle" ] = composante.GetType().GetSensorRotationAngle();
    components_[ &composante ] = &component;
    {
        boost::function< void( const PHY_Sensor& ) > visitor = boost::bind( &AddSensor, boost::ref( component ), _1 );
        composante.ApplyOnSensors( visitor );
    }
    {
        boost::function< void( const PHY_RadarType& ) > visitor = boost::bind( &AddRadar, boost::ref( component ), _1 );
        composante.ApplyOnRadars( visitor );
    }
    {
        boost::function< void( const PHY_ComposantePion&, const PHY_Weapon& ) > visitor = boost::bind( &AddWeapon, boost::ref( component ), _2 );
        composante.ApplyOnWeapons( visitor );
    }
    PHY_RolePion_Composantes::NotifyComposanteAdded( composante );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::NotifyComposanteRemoved
// Created: MCO 2012-03-23
// -----------------------------------------------------------------------------
void RolePion_Composantes::NotifyComposanteRemoved( PHY_ComposantePion& composante )
{
    T_Components::iterator it = components_.find( &composante );
    entity_[ "components" ].RemoveElement( *it->second );
    components_.erase( it );
    PHY_RolePion_Composantes::NotifyComposanteRemoved( composante );
}
