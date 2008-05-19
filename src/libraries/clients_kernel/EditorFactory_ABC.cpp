// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EditorFactory_ABC.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EditorFactory_ABC constructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
EditorFactory_ABC::EditorFactory_ABC()
    : tools::VirtualTemplate< EditorFactory_ABC >( this ) 
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory_ABC destructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
EditorFactory_ABC::~EditorFactory_ABC()
{
    // NOTHING
}
