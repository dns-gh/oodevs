// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "InternalLinks.h"
#include "Agent.h"
#include "Object.h"
#include "Population.h"

const QString InternalLinks::agent_( "agent" );
const QString InternalLinks::object_( "object" );
const QString InternalLinks::population_( "population" );

// -----------------------------------------------------------------------------
// Name: InternalLinks::CreateLink
// Created: AGE 2006-05-12
// -----------------------------------------------------------------------------
QString InternalLinks::CreateLink( const Agent& entity, const QString& message )
{
    return Embed( message, agent_, entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: InternalLinks::CreateLink
// Created: AGE 2006-05-12
// -----------------------------------------------------------------------------
QString InternalLinks::CreateLink( const Object& entity, const QString& message )
{
    return Embed( message, object_, entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: InternalLinks::CreateLink
// Created: AGE 2006-05-12
// -----------------------------------------------------------------------------
QString InternalLinks::CreateLink( const Population& entity, const QString& message )
{
    return Embed( message, population_, entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: InternalLinks::CreateLink
// Created: AGE 2006-05-12
// -----------------------------------------------------------------------------
QString InternalLinks::CreateLink( const QString& type, unsigned long id, const QString& message )
{
    return Embed( message, type, id );
}

// -----------------------------------------------------------------------------
// Name: InternalLinks::Embed
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString InternalLinks::Embed( const QString& message, const QString& classId, unsigned long id )
{
    return QString( "<a href=\"id://%1/%2\">%3</a>" ).arg( classId ).arg( id ).arg( message );
}
