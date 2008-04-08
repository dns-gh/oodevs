// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertiesPanel.h"
#include "PropertiesTableDisplayer.h"
#include "EditorFactory.h"
#include "preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: PropertiesPanelBase::PropertiesPanelBase
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanelBase::PropertiesPanelBase( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : editorFactory_( new EditorFactory( controllers, model, staticModel ) )
    , tableItemDisplayer_( new PropertiesTableDisplayer( staticModel.coordinateConverter_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanelBase::~PropertiesPanelBase
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanelBase::~PropertiesPanelBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesPanel::PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : PropertiesPanelBase( controllers, model, staticModel )
    , gui::PropertiesPanel( parent, controllers, *editorFactory_, *tableItemDisplayer_ )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesPanel destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesPanel::~PropertiesPanel()
{
    // NOTHING
}
