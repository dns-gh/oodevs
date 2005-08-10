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
#include "Tools/Location.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyId
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyId( uint nIdValue, ASN1T_OID& asn )
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
// Name: ASN_Tools::CopyAgent
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgent( uint nIdValue, ASN1T_Agent& asn )
{
    asn = nIdValue;
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
            CopyAgent( *it, asn.elem[ i++ ] );
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
            CopyAgentKnowledge( *it, asn.elem[ i++ ] );
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
            CopyObjectKnowledge( *it, asn.elem[ i++ ] );
    }
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
// Name: ASN_Tools::CopyNumeric
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyNumeric( int nValue, ASN1INT& asn )
{
    asn = nValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPoint
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPoint( const Position& position, ASN1T_CoordUTM& asn )
{
    asn = position.GetMgrsCoordinate().c_str();
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPoint
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPoint( const Position& position, ASN1T_Point& asn )
{
    asn.type = EnumTypeLocalisation::point;
    asn.vecteur_point.n     = 1;

    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[1]; //$$$ RAM
    CopyPoint( position, *pCoord );
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
        CopyPoint( **it, pCoord[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyDirection
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyDirection( uint nDirection, ASN1T_Direction& asn )
{
    asn = nDirection;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyLocation
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyLocation( const Location& location, ASN1T_Localisation& asn )
{
    const T_PositionVector& posVector = location.GetPositionVector();

    asn.type = ( ASN1T_EnumTypeLocalisation )location.GetType();
    asn.vecteur_point.n = posVector.size(); 
    if( posVector.empty() )
        return;

    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[ posVector.size() ]; //$$$ RAM
    asn.vecteur_point.elem = pCoord;
    uint i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, pCoord[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_Localisation& asn )
{
    if( asn.vecteur_point.n > 0 )
        delete [] asn.vecteur_point.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListLocalisation& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );

        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
/** @param  asn 
*/
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListKnowledgeAgent& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListKnowledgeObject& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}
