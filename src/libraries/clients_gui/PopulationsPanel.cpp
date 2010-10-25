// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationsPanel.h"
#include "moc_PopulationsPanel.cpp"
#include "PopulationTypesListView.h"
#include "ValuedDragObject.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PopulationPrototype.h"
#include "preparation/Tools.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationsPanel constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsPanel::PopulationsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "PopulationsPanel", "Populations" ), "PopulationsPanel" )
    , controllers_( controllers )
    , selectedFormation_( 0 )
{
    list_ = new PopulationTypesListView( this, controllers_, types, factory );
    connect( list_, SIGNAL( StartDrag( const kernel::PopulationType* ) ), this, SLOT( OnStartDrag( const kernel::PopulationType* ) ) );
    QHBox* box = new QHBox( this );
    new QLabel( tools::translate( "PopulationsPanel", "Number:" ), box );
    number_ = new QLineEdit( QString::number( 1000 ), box );
    number_->setValidator( new QIntValidator( 1, 1000000, number_ ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsPanel::~PopulationsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::OnStartDrag
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::OnStartDrag( const PopulationType* type )
{
    if( !selectedFormation_ ||!type )
        return;
    prototype_.type_ = type;
    prototype_.number_ = number_->text().toInt();
    QDragObject* drag = new gui::ValuedDragObject( &prototype_, this );
    drag->drag();
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::BeforeSelection
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::BeforeSelection()
{
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::AfterSelection
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::Select
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::Select( const kernel::Formation_ABC& element )
{
    selectedFormation_ = &element;
}
