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
#include "clients_gui/TableItemDisplayer.h"

// -----------------------------------------------------------------------------
// Name: PropertiesBase::PropertiesBase
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesBase::PropertiesBase()
    : editorFactory_( new gui::EditorFactory() )
    , tableItemDisplayer_( new gui::TableItemDisplayer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesBase::~PropertiesPanelBase
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
Properties::Properties( QWidget* parent, kernel::Controllers& controllers )
    : PropertiesBase()
    , gui::PropertiesPanel( parent, controllers, *editorFactory_, *tableItemDisplayer_ )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Properties destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
Properties::~Properties()
{
    // NOTHING
}
