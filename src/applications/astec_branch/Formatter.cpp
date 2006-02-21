// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Formatter.h"

// -----------------------------------------------------------------------------
// Name: FormatterNotImplemented::Error
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void FormatterNotImplemented::Error( Displayer_ABC& displayer, const type_info& type ) const
{
    throw std::runtime_error( std::string( typeid( displayer ).name() ) + " can't display " + type.name() );
}
