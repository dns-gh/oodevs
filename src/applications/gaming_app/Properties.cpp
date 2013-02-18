// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Properties.h"
#include "clients_gui/EditorFactory.h"
#include "clients_gui/PropertiesPanel.h"
#include "clients_gui/PropertyDisplayer.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: PropertiesBase::PropertiesBase
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesBase::PropertiesBase()
    : editorFactory_    ( new gui::EditorFactory() )
    , propertyDisplayer_( new gui::PropertyDisplayer() )
    , propertyComparator_( new gui::PropertyDisplayer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesBase::~PropertiesBase
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesBase::~PropertiesBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Properties constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
Properties::Properties( QWidget* parent, kernel::Controllers& controllers, const gui::GlProxy& glProxy )
    : gui::RichDockWidget( controllers, parent, "properties-panel" )
{
    setWindowTitle( tools::translate( "Properties", "Properties" ) );
    setWidget( new gui::PropertiesPanel( this, controllers, *editorFactory_, *propertyDisplayer_, *propertyComparator_, glProxy ) );
}

// -----------------------------------------------------------------------------
// Name: Properties destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
Properties::~Properties()
{
    // NOTHING
}
