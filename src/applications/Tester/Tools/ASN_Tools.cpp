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
// Name: ASN_Tools::CopyID
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyID( uint nIdValue, ASN1T_OID& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyIDList
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyIDList( const T_IdVector& ids, ASN1T_ListOID& asn )
{
    asn.n    = ids.size();
    if( ids.empty() )
        return;
    asn.elem = new ASN1T_OID[ ids.size() ]; //$$$$ RAM
    uint i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyID( *it, asn.elem[ i++ ] );
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
    if( ids.empty() )
        return;
    asn.elem = new ASN1T_Agent[ ids.size() ]; //$$$$ RAM
    uint i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyAgent( *it, asn.elem[ i++ ] );
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
    if( ids.empty() )
        return;
    asn.elem = new ASN1T_KnowledgeAgent[ ids.size() ]; //$$$$ RAM
    uint i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyAgentKnowledge( *it, asn.elem[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAutomate
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAutomate( uint nIdValue, ASN1T_Automate& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAutomateList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAutomateList( const T_IdVector& ids, ASN1T_ListAutomate& asn )
{
    asn.n = ids.size();
    if( ids.empty() )
        return;
    asn.elem = new ASN1T_Automate[ ids.size() ]; //$$$$ RAM
    uint i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyAutomate( *it, asn.elem[ i++ ] );
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
    if( ids.empty() )
        return;
    asn.elem = new ASN1T_KnowledgeObject[ ids.size() ]; //$$$$ RAM
    uint i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyObjectKnowledge( *it, asn.elem[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyGenObject
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void ASN_Tools::CopyGenObject( uint nIdValue, ASN1T_MissionGenObject& asn )
{
    // TODO: fill in the other fields
    asn.oid_obstacle_planifie = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyGenObjectList
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void ASN_Tools::CopyGenObjectList( const T_IdVector& ids, ASN1T_ListMissionGenObject& asn )
{
    asn.n = ids.size();
    if( ids.empty() )
        return;
    asn.elem = new ASN1T_MissionGenObject[ ids.size() ]; //$$$$ RAM
    uint i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyGenObject( *it, asn.elem[ i++ ] );
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
// Name: ASN_Tools::CopyNumeric
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyNumeric( int nValue, ASN1REAL& asn )
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

    asn.vecteur_point.elem = new ASN1T_CoordUTM[1]; //$$$ RAM
    CopyPoint( position, *asn.vecteur_point.elem );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPointList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPointList( const T_PositionVector& positions, ASN1T_ListPoint& asn )
{
    asn.n = positions.size();
    if( positions.empty() )
        return;

    asn.elem = new ASN1T_Point[ positions.size() ]; //$$$ RAM
    uint i = 0;
    for( CIT_PositionVector it = positions.begin(); it != positions.end(); ++it )
        CopyPoint( **it, asn.elem[ i++ ] );
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

    asn.vecteur_point.elem = new ASN1T_CoordUTM[ posVector.size() ]; //$$$ RAM
    uint i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, asn.vecteur_point.elem[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPathList
/** @param  path 
    @param  asn 
*/
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPathList( const T_PathVector& paths, ASN1T_ListItineraire& asn )
{
    asn.n = paths.size();
    if( paths.empty() )
        return;

    ASN1T_Itineraire* pPath = new ASN1T_Itineraire[ paths.size() ]; //$$$ RAM
    asn.elem = pPath;
    uint i = 0;
    for( CIT_PathVector it = paths.begin(); it != paths.end(); ++it )
        CopyPath( **it, pPath[ i++ ] );
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

    asn.vecteur_point.elem = new ASN1T_CoordUTM[ posVector.size() ]; //$$$ RAM
    uint i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, asn.vecteur_point.elem[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPolygon
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPolygon( const Location& location, ASN1T_Polygon& asn )
{
    const T_PositionVector& posVector = location.GetPositionVector();

    asn.type = EnumTypeLocalisation::polygon;
    asn.vecteur_point.n = posVector.size(); 
    if( posVector.empty() )
        return;

    asn.vecteur_point.elem = new ASN1T_CoordUTM[ posVector.size() ]; //$$$ RAM
    uint i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, asn.vecteur_point.elem[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPolygonList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPolygonList( const T_LocationVector& locations, ASN1T_ListPolygon& asn )
{
    asn.n = locations.size();
    if( locations.empty() )
        return;

    asn.elem = new ASN1T_Polygon[ locations.size() ]; //$$$ RAM
    uint i = 0;
    for( CIT_LocationVector it = locations.begin(); it != locations.end(); ++it )
        CopyPolygon( **it, asn.elem[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyNatureAtlas
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyNatureAtlas( uint nValue, ASN1T_NatureAtlas& asn )
{
    asn.numbits = 11; //$$$ n'importe quoi ...
    asn.data[0] = (char)( nValue >> 8 );
    asn.data[1] = (char)( nValue & 0x00FF );    
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyGDH
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyGDH( uint nTime, ASN1T_GDH& asn )
{
    asn.datation     = nTime;
    asn.qualificatif = EnumGDH_Qualificatif::at;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListAgent& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListAutomate& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_Polygon& asn )
{
    if( asn.vecteur_point.n > 0 )
        delete [] asn.vecteur_point.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListPolygon& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}


// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListPoint& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListItineraire& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
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

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ASN1T_ListMissionGenObject& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n ; ++i )
            delete &asn.elem[ i ];
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* ASN_Tools::ToString( const ASN1T_EnumOrderErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumOrderErrorCode::no_error                        : return "No error";
        case EnumOrderErrorCode::error_invalid_unit              : return "Invalid unit";
        case EnumOrderErrorCode::error_invalid_limit             : return "Invalid limit";
        case EnumOrderErrorCode::error_invalid_lima              : return "Invalid lima";
        case EnumOrderErrorCode::error_invalid_mission           : return "Invalid mission";
        case EnumOrderErrorCode::error_invalid_mission_parameters: return "Invalid mission parameters";
        case EnumOrderErrorCode::error_unit_cannot_receive_order : return "Unit cannot receive this order";
        case EnumOrderErrorCode::error_invalid_order_conduite    : return "Invalid ordre de conduite";
        case EnumOrderErrorCode::error_invalid_order_mission     : return "Invalid ordre de mission";
        case EnumOrderErrorCode::error_invalid_order_initial     : return "Invalid ordre initial";
        case EnumOrderErrorCode::error_invalid_order_conduite_parameters: return "Invalid ordre de conduite parameters";
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::ToString
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
const char* ASN_Tools::ToString( const ASN1T_EnumSetAutomateModeErrorCode& nCode )
{
    switch( nCode )
    {
    case EnumSetAutomateModeErrorCode::no_error                 : return "No error";
    case EnumSetAutomateModeErrorCode::error_invalid_unit       : return "Invalid unit";
    case EnumSetAutomateModeErrorCode::error_unit_not_automate  : return "Unit not automat";
    case EnumSetAutomateModeErrorCode::error_already_embraye    : return "Already engaged";
    case EnumSetAutomateModeErrorCode::error_already_debraye    : return "Already disengaged";
    case EnumSetAutomateModeErrorCode::error_unit_surrendered   : return "Unit surrendered";
    default:
        assert( false );
        return "Unknown";
    }
};