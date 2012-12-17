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

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes constructor
// Created: MCO 2012-09-06
// -----------------------------------------------------------------------------
RolePion_Composantes::RolePion_Composantes()
    : entity_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes constructor
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
RolePion_Composantes::RolePion_Composantes( MIL_Agent_ABC& pion, core::Model& entity )
    : PHY_RolePion_Composantes( pion, false )
    , entity_( &entity[ "components" ] )
{
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
    file & boost::serialization::base_object< PHY_RolePion_Composantes >( *this )
        & entity_;
}

namespace
{
    void AddSensor( core::Model& sensors, const PHY_Sensor& sensor )
    {
        core::Model& s = sensors.AddElement();
        s[ "height" ] = sensor.GetHeight();
        s[ "type" ] = sensor.GetType().GetName();
    }
    void AddRadar( core::Model& radars, const PHY_RadarType& radar )
    {
        core::Model& r = radars.AddElement();
        r[ "type" ] = radar.GetName();
    }
    void AddWeapon( core::Model& weapons, const PHY_Weapon& weapon )
    {
        core::Model& w = weapons.AddElement();
        w[ "type" ] = weapon.GetType().GetID();
    }
}

SWORD_USER_DATA_EXPORT( PHY_ComposantePion* )

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::NotifyComposanteAdded
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
void RolePion_Composantes::NotifyComposanteAdded( PHY_ComposantePion& composante, std::map< const PHY_DotationCategory*, double >* dotations )
{
    core::Model& component = entity_->AddElement();
    component[ "data" ].SetUserData( &composante );
    component[ "major" ] = composante.IsMajor();
    component[ "volume" ] = composante.GetType().GetVolume().GetID();
    component[ "type/sensor-rotation-angle" ] = composante.GetType().GetSensorRotationAngle(); // $$$$ MCO 2012-07-09: type/ ?!
    components_[ &composante ] = &component;
    composante.ApplyOnSensors( boost::bind( &AddSensor, boost::ref( component[ "sensors" ] ), _1 ) );
    composante.ApplyOnRadars( boost::bind( &AddRadar, boost::ref( component[ "radars" ] ), _1 ) );
    composante.ApplyOnWeapons( boost::bind( &AddWeapon, boost::ref( component[ "weapons" ] ), _2 ) );
    PHY_RolePion_Composantes::NotifyComposanteAdded( composante, dotations );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::NotifyComposanteRemoved
// Created: MCO 2012-03-23
// -----------------------------------------------------------------------------
std::map< const PHY_DotationCategory*, double > RolePion_Composantes::NotifyComposanteRemoved( PHY_ComposantePion& composante )
{
    T_Components::iterator it = components_.find( &composante );
    it->second->Remove();
    components_.erase( it );
    return PHY_RolePion_Composantes::NotifyComposanteRemoved( composante );
}
