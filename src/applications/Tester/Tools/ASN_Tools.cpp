// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "ASN_Tools.h"
#include "Tools/Position.h"
#include "Tools/Path.h"


using namespace TEST;

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyId
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyId( uint nIdValue, uint& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyIdList
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyIdList( const T_IdVector& ids, ASN1T_ListOID& asn )
{
    asn.n    = ids.size();
    if( !ids.empty() )
    {
        asn.elem = new ASN1T_OID[ ids.size() ]; //$$$$ RAM
        uint i = 0;
        for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
            CopyId( *it, asn.elem[ i++ ] );
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgentList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgentList( const T_IdVector& ids, ASN1T_ListAgent&  asn )
{
    asn.n = ids.size();
    if( !ids.empty() )
    {
        asn.elem = new ASN1T_Agent[ ids.size() ]; //$$$$ RAM
        uint i = 0;
        for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
            CopyId( *it, asn.elem[ i++ ] );
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgentKnowledge
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgentKnowledge( uint nIdAgent, ASN1T_KnowledgeAgent& asn )
{
    asn = nIdAgent;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgentKnowledgeList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgentKnowledgeList( const T_IdVector& ids, ASN1T_ListKnowledgeAgent& asn )
{
    asn.n = ids.size();
    if( !ids.empty() )
    {
        asn.elem = new ASN1T_KnowledgeAgent[ ids.size() ]; //$$$$ RAM
        uint i = 0;
        for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
            CopyId( *it, asn.elem[ i++ ] );
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyObjectKnowledge
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyObjectKnowledge( uint nIdObject, ASN1T_KnowledgeObject& asn )
{
    asn = nIdObject;
}
// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyObjectKnowledgeList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyObjectKnowledgeList( const T_IdVector& ids, ASN1T_ListKnowledgeObject& asn )
{
    asn.n = ids.size();
    if( !ids.empty() )
    {
        asn.elem = new ASN1T_KnowledgeObject[ ids.size() ]; //$$$$ RAM
        uint i = 0;
        for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
            CopyId( *it, asn.elem[ i++ ] );
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyEnumeration
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyEnumeration( uint nEnumValue, uint& asn )
{
    asn = nEnumValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyBool
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyBool( bool bValue, ASN1BOOL& asn )
{
    asn = bValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyInteger
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyInteger( int nValue, ASN1INT& asn )
{
    asn = nValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPosition
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPosition( const Position& position, ASN1T_CoordUTM& asn )
{
    asn = position.GetMgrsCoordinate().c_str();
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPosition
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPosition( const Position& position, ASN1T_Point& asn )
{
    asn.type = EnumTypeLocalisation::point;
    asn.vecteur_point.n     = 1;

    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[1]; //$$$ RAM
    CopyPosition( position, *pCoord );
    asn.vecteur_point.elem = pCoord;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPath
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPath( const Path& path, ASN1T_Itineraire& asn )
{
    const T_PositionVector& posVector = path.GetPath();

    asn.type            = EnumTypeLocalisation::line;
    asn.vecteur_point.n = posVector.size(); 
    if( posVector.empty() )
        return;

    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[ posVector.size() ]; //$$$ RAM
    asn.vecteur_point.elem = pCoord;
    uint i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPosition( **it, pCoord[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyDirection
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyDirection( uint nDirection, ASN1T_Direction& asn )
{
    asn = nDirection;
}
