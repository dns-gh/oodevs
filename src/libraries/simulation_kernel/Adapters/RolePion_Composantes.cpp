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
#include "Hook.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include <core/Model.h>
#include <core/Facade.h>
#include <core/Convert.h>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RolePion_Composantes )

using namespace sword;

namespace
{
    void ReadWeaponSystem( xml::xistream& xis, core::Model& weapons )
    {
        std::string launcher, ammunition;
        xis >> xml::attribute( "launcher", launcher )
            >> xml::attribute( "munition", ammunition );
        const PHY_WeaponType* type = PHY_WeaponType::FindWeaponType( launcher, ammunition );
        if( ! type )
            throw MASA_EXCEPTION( xis.context() + "Unknown weapon type (" + launcher + ", " + ammunition + ")" );
        weapons.AddElement()[ "type" ] = type->GetID();
    }
    void ReadEquipment( xml::xistream& xis, core::Model& equipments )
    {
        int id;
        std::string protection;
        xis >> xml::attribute( "id", id )
            >> xml::attribute( "protection", protection );
        core::Model& equipement = equipments[ id ];
        equipement[ "protection" ] = protection;
        core::Model& weapons = equipement[ "weapons" ];
        xis >> xml::start( "weapon-systems" )
                >> xml::list( "weapon-system", boost::bind( &ReadWeaponSystem, _1, boost::ref( weapons ) ) );
    }
    DEFINE_HOOK( InitializeEquipments, 2, void, ( const char* xml, SWORD_Model* model ) )
    {
        core::Model& equipments = (*core::Convert( model ))[ "equipments" ];
        // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        xml::xistringstream xis( xml );
        xis >> xml::start( "equipments" )
                >> xml::list( "equipment", boost::bind( &ReadEquipment, _1, boost::ref( equipments ) ) );
        if( GET_PREVIOUS_HOOK( InitializeEquipments ) )
            GET_PREVIOUS_HOOK( InitializeEquipments )( xml, model );
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes constructor
// Created: MCO 2012-09-06
// -----------------------------------------------------------------------------
RolePion_Composantes::RolePion_Composantes()
    : entity_    ( 0 )
    , equipments_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes constructor
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
RolePion_Composantes::RolePion_Composantes( MIL_Agent_ABC& pion, core::Model& model )
    : PHY_RolePion_Composantes( pion, false )
    , entity_    ( &model[ "entities" ][ pion.GetID() ][ "components" ] )
    , equipments_( &model[ "equipments" ] )
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
        & entity_
        & components_;
}

namespace
{
    void AddSensor( core::Model& sensors, const PHY_Sensor& sensor )
    {
        core::Model& s = sensors.AddElement();
        s[ "height" ] = sensor.GetHeight();
        s[ "type" ] = sensor.GetType().GetName();
    }
    void AddRadar( core::Model& radars, const PHY_RadarType& radar, double height )
    {
        core::Model& r = radars.AddElement();
        r[ "type" ] = radar.GetName();
        r[ "height" ] = height;
    }
}

SWORD_USER_DATA_EXPORT( PHY_ComposantePion* )

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::NotifyComposanteAdded
// Created: SLI 2012-03-22
// -----------------------------------------------------------------------------
void RolePion_Composantes::NotifyComposanteAdded( PHY_ComposantePion& composante, T_Dotations* dotations )
{
    core::Model& component = entity_->AddElement();
    component[ "data" ].SetUserData( &composante );
    component[ "major" ] = composante.IsMajor();
    component[ "volume" ] = composante.GetType().GetVolume().GetID();
    component[ "type/sensor-rotation-angle" ] = composante.GetType().GetSensorRotationAngle(); // $$$$ MCO 2012-07-09: type/ ?!
    components_[ &composante ] = &component;
    composante.ApplyOnSensors( boost::bind( &AddSensor, boost::ref( component[ "sensors" ] ), _1 ) );
    composante.ApplyOnRadars( boost::bind( &AddRadar, boost::ref( component[ "radars" ] ), _1, _2 ) );
    const core::Model& equipment = (*equipments_)[ composante.GetType().GetMosID().id() ]; // $$$$ MCO 2013-05-27: could be a link because that info is 'static'
    component[ "weapons" ] = equipment[ "weapons" ];
    component[ "protection" ] = equipment[ "protection" ];
    PHY_RolePion_Composantes::NotifyComposanteAdded( composante, dotations );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Composantes::NotifyComposanteRemoved
// Created: MCO 2012-03-23
// -----------------------------------------------------------------------------
RolePion_Composantes::T_Dotations RolePion_Composantes::NotifyComposanteRemoved( PHY_ComposantePion& composante )
{
    auto it = components_.find( &composante );
    it->second->Remove(); // $$$$ MCO 2013-03-05: won't trigger listeners, post removal event instead ?
    components_.erase( it );
    return PHY_RolePion_Composantes::NotifyComposanteRemoved( composante );
}
