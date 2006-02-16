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

#include "xeumeuleu/xml.h"
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectTypes constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::ObjectTypes( const std::string& scipioXml )
{
    xml::xifstream scipio( scipioXml );
    std::string idFile;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "ClasseIDs", idFile );

    xml::xifstream xis( idFile );
    Reader reader;
    xis >> start( "Classes" )
        >> list( "Classe", reader, &Reader::Read, *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectTypes destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectTypes::~ObjectTypes()
{
    DeleteAll();
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
        objects.Register( nType, *new ObjectType( nType, strObjectName, *pManager ) );
    }
}
