// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertiesPanel.h"
#include "PropertyDisplayer.h"
#include "PropertiesComparator.h"
#include "EditorFactory.h"
#include "preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: PropertiesPanelBase::PropertiesPanelBase
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanelBase::PropertiesPanelBase( kernel::Controllers& controllers,
                                          Model& model,
                                          const StaticModel& staticModel,
                                          const tools::GeneralConfig& config )
    : editorFactory_       ( new EditorFactory( controllers, model, staticModel, config ) )
    , propertiesComparator_( new PropertiesComparator( staticModel.coordinateConverter_ ) )
    , propertiesDisplayer_ ( new PropertyDisplayer( staticModel.coordinateConverter_ ) )
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
PropertiesPanel::PropertiesPanel( QWidget* parent,
                                  kernel::Controllers& controllers,
                                  Model& model,
                                  const StaticModel& staticModel,
                                  const gui::GLView_ABC& view,
                                  const tools::GeneralConfig& config )
    : PropertiesPanelBase( controllers, model, staticModel, config )
    , gui::PropertiesPanel( parent,
                            controllers,
                            *editorFactory_,
                            *propertiesDisplayer_,
                            *propertiesComparator_,
                            view )
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
