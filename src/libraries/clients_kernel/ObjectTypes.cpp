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
#include "ObjectType.h"
#include "DotationType.h"
#include "EquipmentType.h"
#include "NBCAgent.h"
#include "FireClass.h"
#include "MedicalTreatmentType.h"
#include "BreakdownType.h"
#include "WeaponSystemType.h"
#include "tools.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

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

namespace
{
    struct FileReader
    {
        FileReader( xml::xistream& xis ) : xis_( &xis ) {}
        const FileReader& Read( const std::string& tag, std::string& file ) const
        {
            *xis_ >> start( tag ) >> attribute( "file", file ) >> end();
            return *this;
        }
        xml::xistream* xis_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void ObjectTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();

    xml::xifstream scipio( config.GetPhysicalFile() );
    std::string dotations, weaponsystems, equipments, nbc, fire, pannes, objects, medicaltreatment;
    scipio >> start( "physical" );
    FileReader( scipio )
        .Read( "dotations", dotations )
        .Read( "components", equipments )
        .Read( "weapon-systems", weaponsystems )
        .Read( "nbc", nbc )
        .Read( "fire", fire )
        .Read( "medical-treatment", medicaltreatment )
        .Read( "breakdowns", pannes )
        .Read( "objects", objects );
   
    ReadObjectTypes( config.BuildPhysicalChildFile( objects ) );
    ReadDotations( config.BuildPhysicalChildFile( dotations ) );
    ReadWeaponSystems( config.BuildPhysicalChildFile( weaponsystems ) );
    ReadEquipments( config.BuildPhysicalChildFile( equipments ) );
    ReadNBC( config.BuildPhysicalChildFile( nbc ) );
    ReadFire( config.BuildPhysicalChildFile( fire ) );
    ReadMedicalTreatment( config.BuildPhysicalChildFile( medicaltreatment ) );
    ReadBreakdowns( config.BuildPhysicalChildFile( pannes ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ObjectTypes::Purge()
{
    Resolver< BreakdownType >::DeleteAll();
    Resolver2< NBCAgent >::DeleteAll();
    Resolver< EquipmentType >::DeleteAll();
    Resolver< WeaponSystemType, std::string >::DeleteAll();
    Resolver2< DotationType >::DeleteAll();
    StringResolver< ObjectType >::DeleteAll();
    Resolver2< FireClass >::DeleteAll();
    Resolver2< MedicalTreatmentType >::DeleteAll();
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
void ObjectTypes::ReadObjectTypes( const std::string& objects )
{
    xifstream xis( objects );
    xis >> start( "objects" )        
            >> list ( "object", *this, &ObjectTypes::ReadObjectType );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadObjectType
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadObjectType( xml::xistream& xis )
{
    ObjectType* type = new ObjectType( xis );
    StringResolver< ObjectType >::Register( type->GetType(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadDotations
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadDotations( const std::string& dotations )
{
    xifstream xis( dotations );
    xis >> start( "dotations" )
        >> list( "dotation", *this, &ObjectTypes::ReadDotation );
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
void ObjectTypes::ReadWeaponSystems( const std::string& file )
{
    xifstream xis( file );
    xis >> start( "weapons" )
            >> list( "weapon-system", *this, &ObjectTypes::ReadWeaponSystem );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadWeaponSystem
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void ObjectTypes::ReadWeaponSystem( xml::xistream& xis )
{
    WeaponSystemType* type = new WeaponSystemType( xis );
    Resolver< WeaponSystemType, std::string >::Register( type->GetId(), *type );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipments
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipments( const std::string& equipments )
{
    xifstream xis( equipments );
    xis >> start( "elements" )
        >> list( "element", *this, &ObjectTypes::ReadEquipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipment
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipment( xml::xistream& xis )
{
    EquipmentType* equipment = new EquipmentType( xis, *this );
    Resolver< EquipmentType >::Register( equipment->GetId(), *equipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadNBC
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadNBC( const std::string& nbc )
{
    xifstream xis( nbc );
    xis >> start( "nbc" )
            >> start( "agents" )
                >> list( "agent", *this, &ObjectTypes::ReadNBCAgent );
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
void ObjectTypes::ReadFire( const std::string& fire )
{
    xifstream xis( fire );
    xis >> start( "fire-classes" )
            >> list( "class", *this, &ObjectTypes::ReadFireClass );
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
void ObjectTypes::ReadMedicalTreatment( const std::string& medicaltreatment )
{
    xifstream xis( medicaltreatment );
    xis >> start( "medical-treatments" )
            >> list( "medical-treatment", *this, &ObjectTypes::ReadMedicalTreatmentType );
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
void ObjectTypes::ReadBreakdowns( const std::string& breakdowns )
{
    xifstream xis( breakdowns );
    xis >> start( "breakdowns" )
        >> list( "category", *this, &ObjectTypes::ReadBreakdownCategory );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdownCategory
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdownCategory( xml::xistream& xis )
{
    xis >> list( "breakdown", *this, &ObjectTypes::ReadBreakdown );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadBreakdown
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdown( xml::xistream& xis )
{
    BreakdownType* breakdown = new BreakdownType( xis );
    Resolver< BreakdownType >::Register( breakdown->GetId(), *breakdown );
}
