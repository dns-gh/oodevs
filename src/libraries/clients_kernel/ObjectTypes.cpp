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
void ObjectTypes::Load( const ExerciseConfig& config )
{
    Purge();

    xml::xifstream scipio( config.GetPhysicalFile() );
    std::string dotations, equipments, nbc, pannes, objects;
    scipio >> start( "physical" );
    FileReader( scipio )
        .Read( "dotations", dotations )
        .Read( "components", equipments )
        .Read( "nbc", nbc )
        .Read( "breakdowns", pannes )
        .Read( "objects", objects );
   
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
    xis >> start( "objects" )
        >> start( "real-objects" )
        >> list ( "object", *this, &ObjectTypes::ReadObjectType );
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
    Resolver2< ObjectType >::Register( nType, type, *new ObjectType( xis, nType ) );
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
