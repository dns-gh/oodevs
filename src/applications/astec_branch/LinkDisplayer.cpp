// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LinkDisplayer.h"
#include "Agent.h"
#include "Object_ABC.h"
#include "Population.h"
#include "App.h"
#include "World.h"
#include "ActionController.h"

QString LinkDisplayer::link_( "link://" );
QString LinkDisplayer::agent_( "agent/" );
QString LinkDisplayer::population_( "population/" );
QString LinkDisplayer::object_( "object/" );

// -----------------------------------------------------------------------------
// Name: LinkDisplayer constructor
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
LinkDisplayer::LinkDisplayer( ActionController& actionController )
    : actionController_( actionController )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkDisplayer destructor
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
LinkDisplayer::~LinkDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkDisplayer::Trigger
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void LinkDisplayer::Trigger( QString anchor ) const
{
    if( anchor.left( 7 ) != link_ )
        return;
    anchor = anchor.right( anchor.length() - link_.length() );

//    int id = ParseLink( anchor, agent_ );
//    if( id > 0 )
//        TriggerAgent( id );
}

// -----------------------------------------------------------------------------
// Name: LinkDisplayer::ParseLink
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
int LinkDisplayer::ParseLink( const QString& anchor, const QString& type ) const
{
    if( anchor.left( type.length() ) == type )
    {
        bool parsed = false;
        unsigned int linkId = anchor.right( anchor.length() - type.length() ).toInt( &parsed );
        if( parsed )
            return linkId;
    }
    return -1;
}

// $$$$ AGE 2006-03-06: coller un pointeur ??
// -----------------------------------------------------------------------------
// Name: LinkDisplayer::MakeLink
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
QString LinkDisplayer::MakeLink( const QString& type, unsigned int id, const QString& name )
{
    return QString( "<a href=\"link://%1/%2\">%3</a>" ).arg( type ).arg( id ).arg( name );
}

// -----------------------------------------------------------------------------
// Name: LinkDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LinkDisplayer::Call( const Agent& value )
{
    AddToDisplay( MakeLink( agent_, value.GetId(), value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: LinkDisplayer::Call
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void LinkDisplayer::Call( const Object_ABC& value )
{
    AddToDisplay( MakeLink( object_, value.GetId(), value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: LinkDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void LinkDisplayer::Call( const Population& value )
{
    AddToDisplay( MakeLink( population_, value.GetId(), value.GetName().c_str() ) );
}
