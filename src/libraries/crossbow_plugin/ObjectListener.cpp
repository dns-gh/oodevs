// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ObjectListener.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "Line.h"
#include "Point.h"
#include "dispatcher/Network_Def.h"
#include "ScopeEditor.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: ObjectListener constructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::ObjectListener( Database_ABC& database, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_( publisher )
    , table_    ( database.OpenTable( "TacticalObjectPoint", false ) )
    , database_  ( database )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListener destructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::~ObjectListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::Listen
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::Listen()
{
    static const std::string query = "Info <> ''";
    
    Row_ABC* row = table_.Find( query );
    bool bHasUpdates = row != 0;
    while( row )
    {
        SendCreation( *row );
        row = table_.GetNextRow();
    }
    if ( bHasUpdates )
    {
        ScopeEditor editor( database_ );
        table_.DeleteRows( query );
    }
}

namespace
{
    struct GeometrySerializer : public ShapeVisitor_ABC
    {
        GeometrySerializer( ASN1T_Location& asn )
            : asn_( &asn )
        {}
        virtual void Visit( const crossbow::Line& line )
        {
            line.Serialize( *asn_ );
        }
        virtual void Visit( const crossbow::Point& point )
        {
            point.Serialize( *asn_ );
        }

        ASN1T_Location* asn_;
    };

    
    // $$$$ SBO 2007-09-23: hard coded object list
    unsigned int GetType( const std::string& type )
    {
        if( type == "fire" )
            return EnumObjectType::fire;
        if( type == "emergency shelter" )
            return EnumObjectType::emergency_shelter;
        throw std::runtime_error( "unsupported object type" );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::SendCreation
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::SendCreation( const Row_ABC& row )
{
    dispatcher::AsnMsgClientToSimObjectMagicAction asn;
    asn().action.t = T_MsgObjectMagicAction_action_create_object;
    ASN1T_MagicActionCreateObject*& creation = asn().action.u.create_object = new ASN1T_MagicActionCreateObject();
    creation->m.namePresent = 0;
    creation->m.obstacle_typePresent = 0;
    creation->m.reserved_obstacle_activatedPresent = 0;
    creation->m.specific_attributesPresent = 0;
    creation->team = 1; // $$$$ SBO 2007-09-23: Hard coded !!
    creation->type = (ASN1T_EnumObjectType)GetType( boost::get< std::string >( row.GetField( "Info" ) ) );
    GeometrySerializer serializer( creation->location );
    row.GetShape().Accept( serializer );
    asn.Send( publisher_ );
}
