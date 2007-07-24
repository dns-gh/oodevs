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
#include "OrderParameter.h"
#include "dispatcher/Limit.h"
#include "dispatcher/Lima.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
//#include <algorithm>

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

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Serialize
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Serialize( ASN1T_MissionParameter& asn, const kernel::OrderParameter& parameter, const std::string& value ) const
{
    asn.value.t = parameter.GetTypeId();
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
    default:
        break;
    }
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
    asn->coordinates.elem = new ASN1T_CoordUTM[asn->coordinates.n];

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
            asn->coordinates.elem[i - 1] = v.c_str();
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::CleanLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::CleanLocation( ASN1T_Location*& asn ) const
{
    dispatcher::Localisation::AsnDelete( *asn );
    delete asn;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLimits
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeLimits( ASN1T_OrderContext& asn, const std::string& value ) const
{
    asn.m.limite_gauchePresent = 0;
    asn.m.limite_droitePresent = 0;

    // value = id1;id2
    std::stringstream ss( value );
    std::string v;
    while( std::getline( ss, v, ';' ) )
    {
        std::stringstream converter( v );
        unsigned long id = 0;
        converter >> id;
        if( const dispatcher::Limit* limit = model_.GetLimits().Find( id ) )
        {
            if( !asn.m.limite_gauchePresent )
            {
                limit->Send( asn.limite_gauche );
                asn.m.limite_gauchePresent = 1;
            }
            else
            {
                limit->Send( asn.limite_droite );
                asn.m.limite_droitePresent = 1;
            }
        }
    }
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
// Name: OrderParameterSerializer::SerializeLima
// Created: SBO 2007-06-06
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeLima( ASN1T_OrderContext& asn, const std::string& value ) const
{
    if( !asn.limas.n )
        return; // $$$$ SBO 2007-06-06: should not happend...
    if( !asn.limas.elem )
    {
        asn.limas.elem = new ASN1T_LimaOrder[asn.limas.n];
        for( unsigned int i = 0; i < asn.limas.n; ++i )
            asn.limas.elem[i].fonctions.n = 0;
    }
    unsigned int current = 0;
    while( current < asn.limas.n && asn.limas.elem[current].fonctions.n != 0 )
        ++current;
    if( current == asn.limas.n )
        return; // $$$$ SBO 2007-06-06: should not happend either...
    ASN1T_LimaOrder& order = asn.limas.elem[current];

    order.fonctions.n = std::count( value.begin(), value.end(), ';' );
    order.fonctions.elem = new ASN1T_EnumLimaType[order.fonctions.n];

    // value = type;coord1;coord2...
    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ';' ); ++i )
        if( i == 0 )
        {
            std::stringstream converter( v );
            unsigned int id;
            converter >> id;
            if( const dispatcher::Lima* lima = model_.GetLimas().Find( id ) )
                lima->Send( order.lima );
        }
        else
            order.fonctions.elem[i - 1] = ConvertLimaTypeFromString( v );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeDirection
// Created: SBO 2007-06-06
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeDirection( ASN1T_OrderContext& asn, const std::string& value ) const
{
    std::stringstream ss( value );
    unsigned int direction = 0;
    ss >> direction;
    asn.direction_dangereuse = direction;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Clean
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Clean( ASN1T_OrderContext& asn ) const
{
    if( asn.m.limite_droitePresent )
        dispatcher::Localisation::AsnDelete( asn.limite_droite );
    if( asn.m.limite_gauchePresent )
        dispatcher::Localisation::AsnDelete( asn.limite_gauche );
    for( unsigned int i = 0; i < asn.limas.n; ++i )
    {
        delete[] asn.limas.elem[i].fonctions.elem;
        dispatcher::Localisation::AsnDelete( asn.limas.elem[i].lima );
    }
}
