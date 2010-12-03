// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InhabitantsPanel.h"
#include "moc_InhabitantsPanel.cpp"
#include "InhabitantTypesListView.h"
#include "ValuedDragObject.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Tools.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantsPanel::InhabitantsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::InhabitantType >& types, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools, gui::ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "InhabitantsPanel", "Populations" ), "InhabitantsPanel" )
    , controllers_( controllers )
    , selected_( 0 )
{
    list_ = new InhabitantTypesListView( this, controllers_, types, factory );
    connect( list_, SIGNAL( StartDrag( const kernel::InhabitantType* ) ), this, SLOT( OnStartDrag( const kernel::InhabitantType* ) ) );
    QHBox* box = new QHBox( this );
    new QLabel( tools::translate( "InhabitantsPanel", "Number:" ), box );
    number_ = new QLineEdit( QString::number( 1000 ), box );
    number_->setValidator( new QIntValidator( 1, 1000000, number_ ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantsPanel::~InhabitantsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel::NotifyUpdated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantsPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel::OnStartDrag
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantsPanel::OnStartDrag( const InhabitantType* type )
{
    if( !selected_ ||!type )
        return;
    prototype_.type_ = type;
    prototype_.number_ = number_->text().toInt();
    QDragObject* drag = new gui::ValuedDragObject( &prototype_, this );
    drag->drag();
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel::BeforeSelection
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantsPanel::BeforeSelection()
{
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel::AfterSelection
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantsPanel::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel::Select
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantsPanel::Select( const kernel::Formation_ABC& element )
{
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: InhabitantsPanel::Select
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantsPanel::Select( const kernel::Team_ABC& element )
{
    selected_ = &element;
}
