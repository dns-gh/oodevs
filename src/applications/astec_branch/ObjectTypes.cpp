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

#include "xeumeuleu/xml.h"
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectTypes constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::ObjectTypes( const std::string& scipioXml )
{
    xml::xifstream scipio( scipioXml );
    std::string idFile, dotations;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "ClasseIDs", idFile )
                    >> content( "Dotations", dotations );

    xml::xifstream xis( idFile );
    Reader reader;
    xis >> start( "Classes" )
        >> list( "Classe", reader, &Reader::Read, *this );

    ReadDotations( dotations );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::~ObjectTypes()
{
    Resolver< ObjectType >::DeleteAll();
    Resolver< DotationType >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes::Reader::Read
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectTypes::Reader::Read( xml::xistream& xis, ObjectTypes& objects )
{
    std::string strObjectName;
    int nId;

    xis >> attribute( "nom", strObjectName )
        >> attribute( "id", nId );

    ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strObjectName );
    if( nType != -1 )
    {
        objects.objectIds_[ nType ] = nId;
        IDManager*& pManager = objects.managers_[ nId ];
        if( ! pManager )
            pManager = new IDManager( nId );
        objects.Resolver< ObjectType >::Register( nType, *new ObjectType( nType, strObjectName, *pManager ) );
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
