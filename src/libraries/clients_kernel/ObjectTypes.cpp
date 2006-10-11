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
#include "PathTools.h"
#include "ENT/ENT_Tr.h"
#include "xeumeuleu/xml.h"

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
void ObjectTypes::Load( const std::string& scipioXml )
{
    Purge();

    xml::xifstream scipio( scipioXml );
    std::string idFile, dotations, equipments, nbc, pannes;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "ClasseIDs", idFile )
                    >> content( "Dotations", dotations )
                    >> content( "Composantes", equipments )
                    >> content( "NBC", nbc )
                    >> content( "Pannes", pannes );

    xml::xifstream xis( path_tools::BuildChildPath( scipioXml, idFile ) );
    xis >> start( "Classes" )
		>> list( "Classe", *this, &ObjectTypes::ReadObjectTypes );

    ReadDotations( path_tools::BuildChildPath( scipioXml, dotations ) );
    ReadEquipments( path_tools::BuildChildPath( scipioXml, equipments ) );
    ReadNBC( path_tools::BuildChildPath( scipioXml, nbc ) );
    ReadBreakdowns( path_tools::BuildChildPath( scipioXml, pannes ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ObjectTypes::Purge()
{
    Resolver< ObjectType >::DeleteAll();
    Resolver2< DotationType >::DeleteAll();
    Resolver< EquipmentType >::DeleteAll();
    Resolver< NBCAgent >::DeleteAll();
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
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ObjectTypes::ReadObjectTypes( xml::xistream& xis )
{
    std::string strObjectName;
    int nId;

    xis >> attribute( "nom", strObjectName )
        >> attribute( "id", nId );

    int nType = ENT_Tr::ConvertToObjectType( strObjectName );
    if( nType != -1 )
    {
        objectIds_[ nType ] = nId;
        IDManager*& pManager = managers_[ nId ];
        if( ! pManager )
            pManager = new IDManager( nId );
        Resolver< ObjectType >::Register( nType, *new ObjectType( nType, strObjectName.c_str(), *pManager ) );
    }
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
    Resolver< NBCAgent >::Register( nbc->GetId(), *nbc );
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
