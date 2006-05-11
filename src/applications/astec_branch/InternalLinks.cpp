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

// -----------------------------------------------------------------------------
// Name: InternalLinks::Cleanup
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString InternalLinks::Cleanup( const QString& className )
{
    return QString( className ).remove( "class " );
}

// -----------------------------------------------------------------------------
// Name: InternalLinks::GetAddress
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString InternalLinks::GetAddress( const void* add )
{
    return QString::number( reinterpret_cast< int >( add ) );
}
    
// -----------------------------------------------------------------------------
// Name: InternalLinks::Embed
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString InternalLinks::Embed( const QString& message, const QString& classId, const QString& id )
{
    return "<a href=\"id://" + classId + "/" + id + "\">" + message + "</a>";
}
