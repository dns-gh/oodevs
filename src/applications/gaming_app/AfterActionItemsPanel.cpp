// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionItemsPanel.h"
#include "AfterActionItemsCategory.h"
#include "gaming/AfterActionModel.h"
#include "clients_kernel/Controllers.h"
#include <qtoolbox.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionItemsPanel constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItemsPanel::AfterActionItemsPanel( QWidget* parent, Controllers& controllers )
    : QVBox( parent )
    , controllers_( controllers )
{
    toolBox_ = new QToolBox( this );
    setMinimumSize( 140, 480 );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItemsPanel destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItemsPanel::~AfterActionItemsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItemsPanel::NotifyUpdated
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionItemsPanel::NotifyUpdated( const AfterActionModel& model )
{
    toolBox_->addItem( new AfterActionItemsCategory( this, model, "extractor" ),      tr( "Extraction" ) );
    toolBox_->addItem( new AfterActionItemsCategory( this, model, "transformation" ), tr( "Transformation" ) );
    toolBox_->addItem( new AfterActionItemsCategory( this, model, "reduction" ),      tr( "Reduction" ) );
}
