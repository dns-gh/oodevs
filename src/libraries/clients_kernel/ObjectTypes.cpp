// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ObjectTypes.h"
#include "BreakdownType.h"
#include "DotationType.h"
#include "EquipmentType.h"
#include "FireClass.h"
#include "InfrastructureType.h"
#include "MaterialCompositionType.h"
#include "NBCAgent.h"
#include "ObjectType.h"
#include "ResourceNetworkType.h"
#include "RoofShapeType.h"
#include "UrbanTemplateType.h"
#include "VolumeType.h"
#include "WeaponSystemType.h"
#include "LogisticSupplyClass.h"
#include "tools/ExerciseConfig.h"
#include "tools/PhyLoader.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

unsigned int ObjectTypes::nVolumeId = 0;

// -----------------------------------------------------------------------------
// Name: ObjectTypes constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::ObjectTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ObjectTypes::ObjectTypes( const tools::ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Load
// Created: JSR 2010-11-25
// -----------------------------------------------------------------------------
void ObjectTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();
    const tools::PhyLoader& loader = config.GetPhyLoader();
    loader.LoadPhysicalFile( "logistic-supply-classes", boost::bind( &ObjectTypes::ReadLogisticSupplyClasses, this, _1 ) );;
    loader.LoadPhysicalFile( "objects", boost::bind( &ObjectTypes::ReadObjectTypes, this, _1 ) );
    loader.LoadPhysicalFile( "resources", boost::bind( &ObjectTypes::ReadDotations, this, _1 ) );
    loader.LoadPhysicalFile( "volumes", boost::bind( &ObjectTypes::ReadVolumes, this, _1 ) );
    loader.LoadPhysicalFile( "weapon-systems", boost::bind( &ObjectTypes::ReadWeaponSystems, this, _1 ) );
    loader.LoadPhysicalFile( "components", boost::bind( &ObjectTypes::ReadEquipments, this, _1 ) );
    loader.LoadPhysicalFile( "nbc", boost::bind( &ObjectTypes::ReadNBC, this, _1 ) );
    loader.LoadPhysicalFile( "fires", boost::bind( &ObjectTypes::ReadFires, this, _1 ) );
    loader.LoadPhysicalFile( "breakdowns", boost::bind( &ObjectTypes::ReadBreakdowns, this, _1 ) );
    loader.LoadPhysicalFile( "resource-networks", boost::bind( &ObjectTypes::ReadResourceNetworks, this, _1 ) );
    loader.LoadPhysicalFile( "urban", boost::bind( &ObjectTypes::ReadUrbanTypes, this, _1 ) );;
    loader.LoadOptionalPhysicalFile( "urban-templates", boost::bind( &ObjectTypes::ReadUrbanTemplateTypes, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ObjectTypes::Purge()
{
    Resolver2< BreakdownType >::DeleteAll();
    Resolver2< NBCAgent >::DeleteAll();
    Resolver2< EquipmentType >::DeleteAll();
    tools::Resolver< WeaponSystemType, std::string >::DeleteAll();
    Resolver2< DotationType >::DeleteAll();
    tools::StringResolver< ObjectType >::DeleteAll();
    tools::StringResolver< FireClass >::DeleteAll();
    tools::Resolver< VolumeType >::DeleteAll();
    tools::StringResolver< ResourceNetworkType >::DeleteAll();
    tools::StringResolver< MaterialCompositionType >::DeleteAll();
    tools::StringResolver< RoofShapeType >::DeleteAll();
    tools::StringResolver< InfrastructureType >::DeleteAll();
    tools::StringResolver< LogisticSupplyClass >::DeleteAll();
    tools::StringResolver< UrbanTemplateType >::DeleteAll();
    nVolumeId = 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::~ObjectTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadObjectTypes
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadObjectTypes( xml::xistream& xis )
{
    xis >> xml::start( "objects" ) >> list ( "object", *this, &ObjectTypes::ReadObjectType );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadObjectType
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadObjectType( xml::xistream& xis )
{
    ObjectType* type = new ObjectType( xis );
    tools::StringResolver< ObjectType >::Register( type->GetType(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadDotations
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadDotations( xml::xistream& xis )
{
    xis >> xml::start( "resources" ) >> xml::list( "resource", *this, &ObjectTypes::ReadDotation );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadDotation
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadDotation( xml::xistream& xis )
{
    DotationType* dotation = new DotationType( xis, *this );
    Resolver2< DotationType >::Register( dotation->GetId(), dotation->GetName(), *dotation );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadWeaponSystems
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void ObjectTypes::ReadWeaponSystems( xml::xistream& xis )
{
    xis >> xml::start( "weapons" ) >> xml::list( "weapon-system", *this, &ObjectTypes::ReadWeaponSystem );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadWeaponSystem
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void ObjectTypes::ReadWeaponSystem( xml::xistream& xis )
{
    WeaponSystemType* type = new WeaponSystemType( xis, *this );
    tools::Resolver< WeaponSystemType, std::string >::Register( type->GetId(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipments
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipments( xml::xistream& xis )
{
    xis >> xml::start( "equipments" ) >> xml::list( "equipment", *this, &ObjectTypes::ReadEquipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipment
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipment( xml::xistream& xis )
{
    EquipmentType* equipment = new EquipmentType( xis, *this );
    Resolver2< EquipmentType >::Register( equipment->GetId(), equipment->GetName() , *equipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadNBC
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadNBC( xml::xistream& xis )
{
    xis >> xml::start( "nbc" ) >> xml::start( "agents" ) >> xml::list( "agent", *this, &ObjectTypes::ReadNBCAgent );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadNBCAgent
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadNBCAgent( xml::xistream& xis )
{
    NBCAgent* nbc = new NBCAgent( xis );
    Resolver2< NBCAgent >::Register( nbc->GetId(), nbc->GetName(), *nbc );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadFires
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadFires( xml::xistream& xis )
{
    xis >> xml::start( "fires" ) >> xml::list( "fire", *this, &ObjectTypes::ReadFireClasses );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadFireClasses
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadFireClasses( xml::xistream& xis )
{
    FireClass* fire = new FireClass( xis );
    tools::StringResolver< FireClass >::Register( fire->GetName(), *fire );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdowns
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdowns( xml::xistream& xis )
{
    xis >> xml::start( "breakdowns" )
            >> xml::start( "repair-duration-in-man-hours" )
                >> xml::attribute( "value", repairDurationInManHours_ )
            >> xml::end
            >> xml::list( "category", *this, &ObjectTypes::ReadBreakdownCategory )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdownCategory
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdownCategory( xml::xistream& xis )
{
    const std::string category = xis.attribute< std::string >( "name" );
    xis >> xml::list( "breakdown", *this, &ObjectTypes::ReadBreakdown, category );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdown
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdown( xml::xistream& xis, const std::string& category )
{
    BreakdownType* breakdown = new BreakdownType( xis, category, *this );
    Resolver2< BreakdownType >::Register( breakdown->GetId(), breakdown->GetName(), *breakdown );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadVolumes
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void ObjectTypes::ReadVolumes( xml::xistream& xis )
{
    xis >> xml::start( "volumes" ) >> xml::list( "volume", *this, &ObjectTypes::ReadVolume );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadVolume
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void ObjectTypes::ReadVolume( xml::xistream& xis )
{
    VolumeType* volume = new VolumeType( xis, nVolumeId++ );
    tools::Resolver< VolumeType >::Register( volume->GetId(), *volume );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadLogisticSupplyClasses
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void ObjectTypes::ReadLogisticSupplyClasses( xml::xistream& xis )
{
    xis >> xml::start( "logistic-supply-classes" ) >> xml::list( "logistic-supply-class", *this, &ObjectTypes::ReadLogisticSupplyClass );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadLogisticSupplyClass
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void ObjectTypes::ReadLogisticSupplyClass( xml::xistream& xis )
{
    LogisticSupplyClass* data = new LogisticSupplyClass( xis );
    tools::StringResolver< LogisticSupplyClass >::Register( data->GetName(), *data );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadResourceNetworks
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void ObjectTypes::ReadResourceNetworks( xml::xistream& xis )
{
    xis >> xml::start( "resource-networks" ) >> xml::list( "resource-network", *this, &ObjectTypes::ReadResourceNetwork );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadResourceNetwork
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void ObjectTypes::ReadResourceNetwork( xml::xistream& xis )
{
    ResourceNetworkType* type = new ResourceNetworkType( xis );
    tools::StringResolver< ResourceNetworkType >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadUrbanTypes
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadUrbanTypes( xml::xistream& xis )
{
    xis >> xml::start( "urban" )
            >> xml::start( "urban-block-types" )
                >> xml::start( "material-composition-types" )
                    >> xml::list ( "material-composition-type", *this, &ObjectTypes::ReadMaterialCompositionType )
                >> xml::end
                >> xml::start( "roof-shape-types" )
                    >> xml::list ( "roof-shape-type", *this, &ObjectTypes::ReadRoofShapeType )
                >> xml::end
            >> xml::end
            >> xml::optional >> xml::start( "infrastructures" )
                >> xml::list ( "infrastructure", *this, &ObjectTypes::ReadInfrastructureType )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadMaterialCompositionType
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadMaterialCompositionType( xml::xistream& xis )
{
    MaterialCompositionType* type = new MaterialCompositionType( xis );
    StringResolver< MaterialCompositionType >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadRoofShapeType
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadRoofShapeType( xml::xistream& xis )
{
    RoofShapeType* type = new RoofShapeType( xis );
    StringResolver< RoofShapeType >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadInfrastructureType
// Created: SLG 2010-12-30
// -----------------------------------------------------------------------------
void ObjectTypes::ReadInfrastructureType( xml::xistream& xis )
{
    InfrastructureType* type = new InfrastructureType( xis );
    StringResolver< InfrastructureType >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadUrbanTemplateTypes
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void ObjectTypes::ReadUrbanTemplateTypes( xml::xistream& xis )
{
    xis >> xml::start( "templates" ) >> list ( "template", *this, &ObjectTypes::ReadUrbanTemplateType );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadUrbanTemplateType
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void ObjectTypes::ReadUrbanTemplateType( xml::xistream& xis )
{
    UrbanTemplateType* urbanTemplate = new UrbanTemplateType( xis, *this );
    StringResolver< UrbanTemplateType >::Register( urbanTemplate->GetName(), *urbanTemplate );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::GetRepairDurationInManHours
// Created: ABR 2014-05-15
// -----------------------------------------------------------------------------
bool ObjectTypes::GetRepairDurationInManHours() const
{
    return repairDurationInManHours_;
}
