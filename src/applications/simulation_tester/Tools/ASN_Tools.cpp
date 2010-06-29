// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#include "simulation_tester_pch.h"
#include "ASN_Tools.h"
#include "Tools/Position.h"
#include "Tools/Path.h"
#include "Tools/Location.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyID
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyID( unsigned int nIdValue, OID& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyIDList
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyIDList( const T_IdVector& ids, ListOID& asn )
{
    asn.set_n( ids.size() );
    if( ids.empty() )
        return;
    asn.elem = new OID[ ids.size() ]; //$$$$ RAM
    unsigned int i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyID( *it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgent
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgent( unsigned int nIdValue, Unit& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgentList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgentList( const T_IdVector& ids, UnitList&  asn )
{
    asn.set_n( ids.size() );
    if( ids.empty() )
        return;
    asn.elem = new Unit[ ids.size() ]; //$$$$ RAM
    unsigned int i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyAgent( *it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgentKnowledge
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgentKnowledge( unsigned int nIdAgent, MsgUnitKnowledge& asn )
{
    asn = nIdAgent;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAgentKnowledgeList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAgentKnowledgeList( const T_IdVector& ids, UnitKnowledgeList& asn )
{
    asn.set_n( ids.size() );
    if( ids.empty() )
        return;
    asn.elem = new UnitKnowledge[ ids.size() ]; //$$$$ RAM
    unsigned int i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyAgentKnowledge( *it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAutomate
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAutomate( unsigned int nIdValue, Automat& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyAutomateList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyAutomateList( const T_IdVector& ids, AutomatList& asn )
{
    asn.set_n( ids.size() );
    if( ids.empty() )
        return;
    asn.elem = new Automat[ ids.size() ]; //$$$$ RAM
    unsigned int i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyAutomate( *it, asn.elem( i++ ) );
}


// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPopulationKnowledge
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPopulationKnowledge( unsigned int nIdPopulation , PopulationKnowledge&  asn )
{
    asn = nIdPopulation;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyObjectKnowledge
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyObjectKnowledge( unsigned int nIdObject, MsgObjectKnowledge& asn )
{
    asn = nIdObject;
}
// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyObjectKnowledgeList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyObjectKnowledgeList( const T_IdVector& ids, ObjectKnowledgeList& asn )
{
    asn.set_n( ids.size() );
    if( ids.empty() )
        return;
    asn.elem = new ObjectKnowledge[ ids.size() ]; //$$$$ RAM
    unsigned int i = 0;
    for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
        CopyObjectKnowledge( *it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyGenObject
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void ASN_Tools::CopyGenObject( const GenObject& genObject, MsgPlannedWork& asn )
{
    CopyLocation( genObject.GetLocation()  , asn.position );
    asn.set_type( genObject.GetType() );
    asn.set_preliminaire( genObject.GetObstacleSubType() );
    asn.set_densite( genObject.GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyGenObjectList
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void ASN_Tools::CopyGenObjectList( const T_GenObjectVector& genObjects, PlannedWorkList& asn )
{
    asn.set_n( genObjects.size() );
    if( genObjects.empty() )
        return;
    asn.elem = new PlannedWork[ genObjects.size() ]; //$$$$ RAM
    unsigned int i = 0;
    for( CIT_GenObjectVector it = genObjects.begin(); it != genObjects.end(); ++it )
        CopyGenObject( **it, asn.elem( i++ ) );
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
void ASN_Tools::CopyPoint( const Position& position, CoordUTM& asn )
{
    asn = position.GetMgrsCoordinate().c_str();
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPoint
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPoint( const Position& position, Point& asn )
{
    asn.set_type( EnumLocationType::point );
    asn.vecteur_point().set_n( 1 );

    asn.mutable_vecteur_point()->mutable_elem() = new CoordUTM[1]; //$$$ RAM
    CopyPoint( position, *asn.mutable_vecteur_point()->mutable_elem() );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPointList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPointList( const T_PositionVector& positions, PointList& asn )
{
    asn.set_n( positions.size() );
    if( positions.empty() )
        return;

    asn.elem = new Point[ positions.size() ]; //$$$ RAM
    unsigned int i = 0;
    for( CIT_PositionVector it = positions.begin(); it != positions.end(); ++it )
        CopyPoint( **it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPath
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPath( const Path& path, Path& asn )
{
    const T_PositionVector& posVector = path.GetPath();

    asn.set_type( EnumLocationType::line );
    asn.vecteur_point().set_n( posVector.size() );
    if( posVector.empty() )
        return;

    asn.mutable_vecteur_point()->mutable_elem() = new CoordUTM[ posVector.size() ]; //$$$ RAM
    unsigned int i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, asn.mutable_vecteur_point()->mutable_elem()( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPathList
/** @param  path
    @param  asn
*/
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPathList( const T_PathVector& paths, PathList& asn )
{
    asn.set_n( paths.size() );
    if( paths.empty() )
        return;

    Path* pPath = new Path[ paths.size() ]; //$$$ RAM
    asn.set_elem( pPath );
    unsigned int i = 0;
    for( CIT_PathVector it = paths.begin(); it != paths.end(); ++it )
        CopyPath( **it, pPath[ i++ ] );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyDirection
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void ASN_Tools::CopyDirection( unsigned int nDirection, Heading& asn )
{
    asn = nDirection;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyLocation
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyLocation( const Location& location, Location& asn )
{
    const T_PositionVector& posVector = location.GetPositionVector();

    asn.type = ( EnumLocationType )location.GetType();
    asn.vecteur_point().set_n( posVector.size() );
    if( posVector.empty() )
        return;

    asn.mutable_vecteur_point()->mutable_elem() = new CoordUTM[ posVector.size() ]; //$$$ RAM
    unsigned int i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, asn.mutable_vecteur_point()->mutable_elem()( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyLocationList
// Created: SBO 2006-03-15
// -----------------------------------------------------------------------------
void ASN_Tools::CopyLocationList( const T_LocationVector& locations, LocationList& asn )
{
    asn.set_n( locations.size() );
    if( locations.empty() )
        return;

    asn.elem = new Location[ locations.size() ]; //$$$ RAM
    unsigned int i = 0;
    for( CIT_LocationVector it = locations.begin(); it != locations.end(); ++it )
        CopyLocation( **it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPolygon
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPolygon( const Location& location, Polygon& asn )
{
    const T_PositionVector& posVector = location.GetPositionVector();

    asn.set_type( EnumLocationType::polygon );
    asn.vecteur_point().set_n( posVector.size() );
    if( posVector.empty() )
        return;

    asn.mutable_vecteur_point()->mutable_elem() = new CoordUTM[ posVector.size() ]; //$$$ RAM
    unsigned int i = 0;
    for( CIT_PositionVector it = posVector.begin(); it != posVector.end(); ++it )
        CopyPoint( **it, asn.mutable_vecteur_point()->mutable_elem()( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyPolygonList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::CopyPolygonList( const T_LocationVector& locations, PolygonList& asn )
{
    asn.set_n( locations.size() );
    if( locations.empty() )
        return;

    asn.elem = new Polygon[ locations.size() ]; //$$$ RAM
    unsigned int i = 0;
    for( CIT_LocationVector it = locations.begin(); it != locations.end(); ++it )
        CopyPolygon( **it, asn.elem( i++ ) );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyNatureAtlas
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyNatureAtlas( unsigned int nValue, AtlasNature& asn )
{
    asn.set_numbits( 11 ); //$$$ n'importe quoi ...
    asn.data[0] = (char)( nValue >> 8 );
    asn.data[1] = (char)( nValue & 0x00FF );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyGDH
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void ASN_Tools::CopyGDH( unsigned int nTime, DateTime& asn )
{
    asn.set_datation( nTime );
    asn.set_qualificatif( EnumGDH_Qualificatif::at );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyMedicalPriorities
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void ASN_Tools::CopyMedicalPriorities( const T_IdVector& ids, LogMedicalPriorities& asn )
{
    asn.set_n( ids.size() );
    if( !ids.empty() )
    {
        asn.elem = new EnumHumanWound[ ids.size() ];
        unsigned int i = 0;
        for( CIT_IdVector it = ids.begin(); it != ids.end(); ++it )
            asn.elem( i++ ) = (EnumHumanWound)*it;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::CopyDotationDType
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
void ASN_Tools::CopyDotationDType( unsigned int nIdValue, DotationType& asn )
{
    asn = nIdValue;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( LogMedicalPriorities& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( UnitList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( AutomatList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( Polygon& asn )
{
    if( asn.vecteur_point().elem_size() > 0 )
        delete [] asn.mutable_vecteur_point()->mutable_elem();
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( PolygonList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem( i ) );
        delete [] asn.elem;
    }
}


// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( PointList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem( i ) );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( PathList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem( i ) );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( LocationList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem( i ) );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( UnitKnowledgeList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( ObjectKnowledgeList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( MsgPlannedWork& asn )
{
    Delete( asn.position );
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Tools::Delete( PlannedWorkList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n ; ++i )
            Delete( asn.elem( i ) );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* ASN_Tools::ToString( const EnumOrderErrorCode& nCode )
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
const char* ASN_Tools::ToString( const EnumSetAutomatModeErrorCode& nCode )
{
    switch( nCode )
    {
    case EnumSetAutomatModeErrorCode::no_error                 : return "No error";
    case EnumSetAutomatModeErrorCode::error_invalid_unit       : return "Invalid unit";
    case EnumSetAutomatModeErrorCode::error_unit_not_automate  : return "Unit not automat";
    case EnumSetAutomatModeErrorCode::error_already_embraye    : return "Already engaged";
    case EnumSetAutomatModeErrorCode::error_already_debraye    : return "Already disengaged";
    case EnumSetAutomatModeErrorCode::error_unit_surrendered   : return "Unit surrendered";
    default:
        assert( false );
        return "Unknown";
    }
};
