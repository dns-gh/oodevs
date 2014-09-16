// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InternalLinks.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InternalLinks::CreateLink
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString InternalLinks::CreateLink( const Entity_ABC& entity, const QString& message )
{
    return Embed( message, entity.GetTypeName().c_str(), entity.GetId() );
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
    return QString( "<a href=\"id://%1/%2\">%3</a>" ).arg( Qt::escape( classId ) ).arg( id ).arg( Qt::escape( message ) );
}
