// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Logger_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Logger_ABC::Ender
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger_ABC::LogElement::Ender::Ender( Logger_ABC& base, std::stringstream& output )
    : base_  ( &base )
    , output_( &output )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger_ABC::~Ender
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger_ABC::LogElement::Ender::~Ender()
{
    base_->End( *output_ );
}

// -----------------------------------------------------------------------------
// Name: Logger_ABC::LogElement
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger_ABC::LogElement::LogElement( Logger_ABC& base, std::stringstream& output )
    : ender_( new Ender( base, output ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger_ABC::LogElement::~LogElement
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger_ABC::LogElement::~LogElement()
{
    // NOTHING
}
