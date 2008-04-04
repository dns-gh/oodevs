// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderParameterSerializer.h"
#include "clients_kernel/OrderParameter.h"
#include "dispatcher/Limit.h"
#include "dispatcher/Lima.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include <geocoord/Geodetic.h>
#include <geocoord/MGRS.h>
#include <cmath>

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameterSerializer::OrderParameterSerializer( const dispatcher::Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameterSerializer::~OrderParameterSerializer()
{
    // NOTHING
}

namespace
{
    int ResolveType( const std::string& type )
    {
        std::string lowerType = type;
        std::transform( type.begin(), type.end(), lowerType.begin(), &tolower );
        if( lowerType == "location" )
            return T_MissionParameter_value_location;
        else if( lowerType == "point" )
            return T_MissionParameter_value_point;
        else if( lowerType == "polygon" )
            return T_MissionParameter_value_polygon;
        else if( lowerType == "path" )
            return T_MissionParameter_value_path;
        else if( lowerType == "bool" )
            return T_MissionParameter_value_aBool;
        else if( lowerType == "automate" ) // $$$$ SBO 2007-07-24: 
            return T_MissionParameter_value_automat;
        else if( lowerType == "dangerdirection" )
            return T_MissionParameter_value_heading;
        else if( lowerType == "phaselinelist" )
            return T_MissionParameter_value_limasOrder;
        else if( lowerType == "limit" )
            return T_MissionParameter_value_line;
        else if( lowerType == "intelligencelist" )
            return T_MissionParameter_value_intelligenceList;
        throw std::runtime_error( "Unsupported parameter type" );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Serialize
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Serialize( ASN1T_MissionParameter& asn, const kernel::OrderParameter& parameter, const std::string& value ) const
{
    asn.value.t = ResolveType( parameter.GetType() );
    switch( asn.value.t )
    {
    case T_MissionParameter_value_point:
        SerializeLocation( asn.value.u.point, value );
        break;
    case T_MissionParameter_value_polygon:
        SerializeLocation( asn.value.u.polygon, value );
        break;
    case T_MissionParameter_value_location:
        SerializeLocation( asn.value.u.location, value );
        break;
    case T_MissionParameter_value_path:
        SerializeLocation( asn.value.u.path, value );
        break;
    case T_MissionParameter_value_automat:
        SerializeAutomat( asn.value.u.automat, value );
        break;
    case T_MissionParameter_value_aBool:
        SerializeBool( asn.value.u.aBool, value );
        break;
    case T_MissionParameter_value_heading:
        SerializeDirection( asn.value.u.heading, value );
        break;
    case T_MissionParameter_value_limasOrder:
        SerializePhaseLines( asn.value.u.limasOrder, value );
        break;
    case T_MissionParameter_value_line:
        SerializeLimit( asn.value.u.line, value );
        break;
    case T_MissionParameter_value_intelligenceList:
        SerializeIntelligenceList( asn.value.u.intelligenceList, value );
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Clean
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Clean( ASN1T_MissionParameter& asn ) const
{
    switch( asn.value.t )
    {
    case T_MissionParameter_value_point:
        CleanLocation( asn.value.u.point );
        break;
    case T_MissionParameter_value_polygon:
        CleanLocation( asn.value.u.polygon );
        break;
    case T_MissionParameter_value_location:
        CleanLocation( asn.value.u.location );
        break;
    case T_MissionParameter_value_path:
        CleanLocation( asn.value.u.path );
        break;
    case T_MissionParameter_value_line:
        CleanLocation( asn.value.u.line );
        break;
    case T_MissionParameter_value_limasOrder:
        if( asn.value.u.limasOrder && asn.value.u.limasOrder->elem )
        {
            for( unsigned int i = 0; i < asn.value.u.limasOrder->n; ++i )
                delete[] asn.value.u.limasOrder->elem[i].fonctions.elem;
            delete[] asn.value.u.limasOrder->elem;
            delete asn.value.u.limasOrder;
        }
    case T_MissionParameter_value_intelligenceList:
        if( asn.value.u.intelligenceList )
        {
            delete[] asn.value.u.intelligenceList->elem;
            delete asn.value.u.intelligenceList;
        }
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::CleanLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::CleanLocation( ASN1T_Location*& asn ) const
{
    delete asn;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeDirection
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeDirection( ASN1T_Heading& asn, const std::string& value ) const
{
    std::stringstream ss( value );
    ss >> asn;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeAutomat
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeAutomat( ASN1T_Automat& asn, const std::string& value ) const
{
    std::stringstream ss( value );
    unsigned long id;
    ss >> id;
    if( const dispatcher::Agent* agent = model_.GetAgents().Find( id ) )
        asn = agent->GetAutomat().GetID();
     // $$$$ SBO 2007-06-07: else...
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeBool
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeBool( ASN1BOOL& asn, const std::string& value ) const
{
    asn = value == "true" ? true : false;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeLocation( ASN1T_Location*& asn, const std::string& value ) const
{
    asn = new ASN1T_Location();
    asn->coordinates.n = std::count( value.begin(), value.end(), ';' );
    asn->coordinates.elem = new ASN1T_CoordLatLong[asn->coordinates.n];

    // value = type;coord1;coord2...
    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ';' ); ++i )
        if( i == 0 )
        {
            std::stringstream converter( v );
            unsigned int type;
            converter >> type;
            asn->type = ASN1T_EnumLocationType( type );
        }
        else
        {
            geocoord::MGRS mgrs( v );
            geocoord::Geodetic geodetic( mgrs );
            asn->coordinates.elem[i - 1].latitude  = geodetic.GetLatitude() * std::acos( -1. ) / 180;
            asn->coordinates.elem[i - 1].longitude = geodetic.GetLongitude() * std::acos( -1. ) / 180;
        }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLimit
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeLimit( ASN1T_Line*& asn, const std::string& value ) const
{
    unsigned long id = 0;
    std::stringstream ss( value );
    ss >> id;
    if( const dispatcher::Limit* limit = model_.GetLimits().Find( id ) )
    {
        asn = new ASN1T_Line();
        limit->Send( *asn );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLines( ASN1T_LimasOrder*& asn, const std::string& value ) const
{
    // $$$$ SBO 2008-03-04: value=id1,func1,func2;id2,func1;...
    asn = new ASN1T_LimasOrder();
    asn->n = std::count( value.begin(), value.end(), ';' );
    if( !asn->n )
        return;
    asn->elem = new ASN1T_LimaOrder[asn->n];
    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ';' ); ++i )
        SerializePhaseLine( asn->elem[i], v );
}

namespace
{
    ASN1T_EnumLimaType ConvertLimaTypeFromString( const std::string& type )
    {
        static const std::string functions[] = { "LD", "LCA", "LC", "LI", "LO", "LCAR", "LR", "LDM", "LFM", "LIA" };
        for( int i = 0; i < 10; ++i )
        if( functions[i] == type )
            return (ASN1T_EnumLimaType)i;
        return (ASN1T_EnumLimaType)-1;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLine
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLine( ASN1T_LimaOrder& asn, const std::string& value ) const
{
    // $$$$ SBO 2008-03-10: value=id,func1,func2
    asn.fonctions.n = std::count( value.begin(), value.end(), ',' );
    if( asn.fonctions.n )
        asn.fonctions.elem = new ASN1T_EnumLimaType[asn.fonctions.n];

    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ',' ); ++i )
        if( i == 0 )
        {
            std::stringstream converter( v );
            unsigned int id;
            converter >> id;
            if( const dispatcher::Lima* lima = model_.GetLimas().Find( id ) )
                lima->Send( asn.lima );
        }
        else
            asn.fonctions.elem[i - 1] = ConvertLimaTypeFromString( v );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeIntelligenceList
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeIntelligenceList( ASN1T_IntelligenceList*& asn, const std::string& /*value*/ ) const
{
    // $$$$ SBO 2008-03-10: Not Supported
    asn = new ASN1T_IntelligenceList();
    asn->n = 0;
    asn->elem = 0;
}
