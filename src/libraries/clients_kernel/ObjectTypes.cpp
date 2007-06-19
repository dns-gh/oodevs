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
#include "BreakdownType.h"
#include "ExerciseConfig.h"
#include "xeumeuleu/xml.h"
#include "tools.h"

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
// Name: ObjectTypes::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void ObjectTypes::Load( const ExerciseConfig& config )
{
    Purge();

    xml::xifstream scipio( config.GetPhysicalFile() );
    std::string dotations, equipments, nbc, pannes, objects;
    scipio >> start( "physical" )
                >> content( "Dotations", dotations )
                >> content( "Composantes", equipments )
                >> content( "NBC", nbc )
                >> content( "Pannes", pannes )
                >> content( "Objets", objects );
   
    ReadObjectTypes( config.BuildPhysicalChildFile( objects ) );
    ReadDotations( config.BuildPhysicalChildFile( dotations ) );
    ReadEquipments( config.BuildPhysicalChildFile( equipments ) );
    ReadNBC( config.BuildPhysicalChildFile( nbc ) );
    ReadBreakdowns( config.BuildPhysicalChildFile( pannes ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ObjectTypes::Purge()
{
    Resolver2< ObjectType >::DeleteAll();
    Resolver2< DotationType >::DeleteAll();
    Resolver< EquipmentType >::DeleteAll();
    Resolver2< NBCAgent >::DeleteAll();
    Resolver< BreakdownType >::DeleteAll();
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
    xis >> start( "Objets" )
        >> start( "ObjetsReels" )
        >> list ( "Objet", *this, &ObjectTypes::ReadObjectType );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadObjectType
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ObjectTypes::ReadObjectType( xml::xistream& xis )
{
    std::string type;
    xis >> attribute( "type", type );
    int nType = tools::ObjectTypeFromString( type );
    Resolver2< ObjectType >::Register( nType, type.c_str(), *new ObjectType( xis, nType ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadDotations
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadDotations( const std::string& dotations )
{
    xifstream xis( dotations );
    xis >> start( "Dotations" )
        >> list( "Dotation", *this, &ObjectTypes::ReadDotation );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadDotation
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadDotation( xml::xistream& xis )
{
    std::string dotationName;
    xis >> attribute( "nom", dotationName )
        >> start( "Categories" )
            >> list( "Categorie", *this, &ObjectTypes::ReadCategory, dotationName )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadCategory
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadCategory( xml::xistream& xis, const std::string& name )
{
    DotationType* dotation = new DotationType( name.c_str(), xis );
    Resolver2< DotationType >::Register( dotation->GetId(), dotation->GetCategory(), *dotation );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipments
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipments( const std::string& equipments )
{
    xifstream xis( equipments );
    xis >> start( "Composantes" )
        >> list( "Composante", *this, &ObjectTypes::ReadEquipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipment
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipment( xml::xistream& xis )
{
    EquipmentType* equipment = new EquipmentType( xis );
    Resolver< EquipmentType >::Register( equipment->GetId(), *equipment );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadNBC
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void ObjectTypes::ReadNBC( const std::string& nbc )
{
    xifstream xis( nbc );
    xis >> start( "NBC" )
            >> start( "AgentsNBC" )
                >> list( "AgentNBC", *this, &ObjectTypes::ReadNBCAgent );
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
// Name: ObjectTypes::ReadBreakdowns
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ObjectTypes::ReadBreakdowns( const std::string& breakdowns )
{
    xifstream xis( breakdowns );
    xis >> start( "Pannes" )
            >> start( "Types" )
                >> start( "NTI1" )
                    >> list( "Panne", *this, &ObjectTypes::ReadBreakdown )
                >> end()
                >> start( "NTI2" )
                    >> list( "Panne", *this, &ObjectTypes::ReadBreakdown )
                >> end()
                >> start( "NTI3" )
                    >> list( "Panne", *this, &ObjectTypes::ReadBreakdown )
                >> end();

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
