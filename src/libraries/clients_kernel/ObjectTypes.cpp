// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ObjectTypes.h"
#include "BreakdownType.h"
#include "DotationType.h"
#include "EquipmentType.h"
#include "FileLoader.h"
#include "FireClass.h"
#include "MedicalTreatmentType.h"
#include "NBCAgent.h"
#include "ObjectType.h"
#include "ResourceNetworkType.h"
#include "VolumeType.h"
#include "WeaponSystemType.h"
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
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void ObjectTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();
    FileLoader( config )
        .Load( "objects", boost::bind( &ObjectTypes::ReadObjectTypes, this, _1 ) )
        .Load( "dotations", boost::bind( &ObjectTypes::ReadDotations, this, _1 ) )
        .Load( "volumes", boost::bind( &ObjectTypes::ReadVolumes, this, _1 ) )
        .Load( "weapon-systems", boost::bind( &ObjectTypes::ReadWeaponSystems, this, _1 ) )
        .Load( "components", boost::bind( &ObjectTypes::ReadEquipments, this, _1 ) )
        .Load( "nbc", boost::bind( &ObjectTypes::ReadNBC, this, _1 ) )
        .Load( "fire", boost::bind( &ObjectTypes::ReadFire, this, _1 ) )
        .Load( "medical-treatment", boost::bind( &ObjectTypes::ReadMedicalTreatment, this, _1 ) )
        .Load( "breakdowns", boost::bind( &ObjectTypes::ReadBreakdowns, this, _1 ) )
        .Load( "resource-networks", boost::bind( &ObjectTypes::ReadResourceNetworks, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ObjectTypes::Purge()
{
    tools::Resolver< BreakdownType >::DeleteAll();
    Resolver2< NBCAgent >::DeleteAll();
    tools::Resolver< EquipmentType >::DeleteAll();
    tools::Resolver< WeaponSystemType, std::string >::DeleteAll();
    Resolver2< DotationType >::DeleteAll();
     tools::StringResolver< ObjectType >::DeleteAll();
    Resolver2< FireClass >::DeleteAll();
    Resolver2< MedicalTreatmentType >::DeleteAll();
    tools::Resolver< VolumeType >::DeleteAll();
    tools::StringResolver< ResourceNetworkType >::DeleteAll();
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
    xis >> xml::start( "dotations" ) >> xml::list( "dotation", *this, &ObjectTypes::ReadDotation );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadDotation
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadDotation( xml::xistream& xis )
{
    DotationType* dotation = new DotationType( xis );
    Resolver2< DotationType >::Register( dotation->GetId(), dotation->GetCategory(), *dotation );
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
    xis >> xml::start( "elements" ) >> xml::list( "element", *this, &ObjectTypes::ReadEquipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipment
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipment( xml::xistream& xis )
{
    EquipmentType* equipment = new EquipmentType( xis, *this );
    tools::Resolver< EquipmentType >::Register( equipment->GetId(), *equipment );
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
// Name: ObjectTypes::ReadFire
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadFire( xml::xistream& xis )
{
    xis >> xml::start( "fire-classes" ) >> xml::list( "class", *this, &ObjectTypes::ReadFireClass );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadFireClass
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadFireClass( xml::xistream& xis )
{
    FireClass* fire = new FireClass( xis );
    Resolver2< FireClass >::Register( fire->GetId(), fire->GetName(), *fire );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadMedicalTreatment
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadMedicalTreatment( xml::xistream& xis )
{
    xis >> xml::start( "medical-treatments" ) >> xml::list( "medical-treatment", *this, &ObjectTypes::ReadMedicalTreatmentType );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadMedicalTreatmentType
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadMedicalTreatmentType( xml::xistream& xis )
{
    MedicalTreatmentType* medicaltreatment = new MedicalTreatmentType( xis );
    Resolver2< MedicalTreatmentType >::Register( medicaltreatment->GetId(), medicaltreatment->GetName(), *medicaltreatment );
}
// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdowns
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdowns( xml::xistream& xis )
{
    xis >> xml::start( "breakdowns" ) >> xml::list( "category", *this, &ObjectTypes::ReadBreakdownCategory );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdownCategory
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdownCategory( xml::xistream& xis )
{
    xis >> xml::list( "breakdown", *this, &ObjectTypes::ReadBreakdown );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdown
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdown( xml::xistream& xis )
{
    BreakdownType* breakdown = new BreakdownType( xis );
    tools::Resolver< BreakdownType >::Register( breakdown->GetId(), *breakdown );
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
