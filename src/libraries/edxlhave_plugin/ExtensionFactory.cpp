// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "ExtensionFactory.h"
#include "ObjectExtension.h"
#include "dispatcher/Object.h"

using namespace plugins::edxl;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( ReportBuilder_ABC& builder, const dispatcher::Model_ABC& model )
    : builder_( builder )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Object& object )
{
    object.Attach( *new ObjectExtension( /*object,*/ builder_, model_ ) );
}
