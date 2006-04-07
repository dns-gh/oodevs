// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectTypes.h"
#include "ObjectType.h"
#include "DotationType.h"
#include "EquipmentType.h"
#include "NBCAgent.h"
#include "BreakdownType.h"
#include "App.h"

#include "xeumeuleu/xml.h"
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectTypes constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::ObjectTypes( const std::string& scipioXml )
{
    xml::xifstream scipio( scipioXml );
    std::string idFile, dotations, equipments, nbc, pannes;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "ClasseIDs", idFile )
                    >> content( "Dotations", dotations )
                    >> content( "Composantes", equipments )
                    >> content( "NBC", nbc )
                    >> content( "Pannes", pannes );

    xml::xifstream xis( App::BuildChildPath( scipioXml, idFile ) );
    xis >> start( "Classes" )
        >> list( "Classe", *this, ReadObjectTypes );

    ReadDotations( App::BuildChildPath( scipioXml, dotations ) );
    ReadEquipments( App::BuildChildPath( scipioXml, equipments ) );
    ReadNBC( App::BuildChildPath( scipioXml, nbc ) );
    ReadBreakdowns( App::BuildChildPath( scipioXml, pannes ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::~ObjectTypes()
{
    Resolver< ObjectType >::DeleteAll();
    Resolver< DotationType >::DeleteAll();
    Resolver< EquipmentType >::DeleteAll();
    Resolver< NBCAgent >::DeleteAll();
    Resolver< BreakdownType >::DeleteAll();
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

    ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strObjectName );
    if( nType != -1 )
    {
        objectIds_[ nType ] = nId;
        IDManager*& pManager = managers_[ nId ];
        if( ! pManager )
            pManager = new IDManager( nId );
        Resolver< ObjectType >::Register( nType, *new ObjectType( nType, strObjectName, *pManager ) );
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
        >> list( "Dotation", *this, ReadDotation );
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
            >> list( "Categorie", *this, ReadCategory, dotationName )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadCategory
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadCategory( xml::xistream& xis, const std::string& name )
{
    DotationType* dotation = new DotationType( name, xis );
    Resolver< DotationType >::Register( dotation->GetId(), *dotation );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::ReadEquipments
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void ObjectTypes::ReadEquipments( const std::string& equipments )
{
    xifstream xis( equipments );
    xis >> start( "Composantes" )
        >> list( "Composante", *this, ReadEquipment );
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
                >> list( "AgentNBC", *this, ReadNBCAgent );
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
                    >> list( "Panne", *this, ReadBreakdown )
                >> end()
                >> start( "NTI2" )
                    >> list( "Panne", *this, ReadBreakdown )
                >> end()
                >> start( "NTI3" )
                    >> list( "Panne", *this, ReadBreakdown )
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
