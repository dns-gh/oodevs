// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GhostsPanel.h"
#include "moc_GhostsPanel.cpp"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolRule.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/UnitPreviewIcon.h"
#include "clients_gui/ValuedDragObject.h"

#include "GhostSymbolEditor.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GhostsPanel constructor
// Created: ABR 2011-10-12
// -----------------------------------------------------------------------------
GhostsPanel::GhostsPanel( QWidget* parent, gui::PanelStack_ABC& panel, Controllers& controllers, SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy )
    : InfoPanel_ABC( parent, panel, tr( "Phantom" ), "GhostsPanel" )
    , controllers_        ( controllers )
    , symbolsFactory_     ( symbolsFactory )
{
    // Base
    QWidget* box = new QWidget( this );
    QGridLayout* layout = new QGridLayout( box );
    layout->setAlignment( Qt::AlignTop );
    layout->setMargin( 10 );
    layout->setSpacing( 10 );
    setWidget( box );
    // Name
    layout->addWidget( new QLabel( tr( "Name:" ), this ), 0, 0 );
    nameLineEdit_ = new QLineEdit( this );
    connect( nameLineEdit_, SIGNAL( textChanged( const QString& ) ), SLOT( UpdateWarning() ) );
    layout->addWidget( nameLineEdit_, 0, 1 );
    // Type
    layout->addWidget( new QLabel( tr( "Type:" ), this ), 1, 0 );
    typeLineEdit_ = new QLineEdit( this );
    connect( typeLineEdit_, SIGNAL( textChanged( const QString& ) ), SLOT( UpdateWarning() ) );
    layout->addWidget( typeLineEdit_, 1, 1 );

    // Symbol Editor
    symbolEditor_ = new GhostSymbolEditor( layout, 2, controllers_, symbolsFactory_, icons, colorStrategy );
    connect( symbolEditor_, SIGNAL( LevelChanged() ), SLOT( UpdateWarning() ) );
    connect( symbolEditor_, SIGNAL( StartDrag() ), SLOT( IconDragged() ) );

    // Warning
    warningLabel_ = new QLabel( this );
    layout->addWidget( warningLabel_, 10, 0, 1, 2, Qt::AlignCenter );

    // Configuration
    controllers_.Register( *this );

    UpdateWarning();
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel destructor
// Created: ABR 2011-10-12
// -----------------------------------------------------------------------------
GhostsPanel::~GhostsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::NotifyUpdated
// Created: ABR 2011-10-12
// -----------------------------------------------------------------------------
void GhostsPanel::NotifyUpdated( const ModelLoaded& )
{
    nameLineEdit_->clear();
    typeLineEdit_->clear();
    nameLineEdit_->setPlaceholderText( tr( "Enter phantom name here" ) );
    typeLineEdit_->setPlaceholderText( tr( "Enter phantom type here" ) );
    Show();
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::Load
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void GhostsPanel::Load()
{
    symbolEditor_->Reset();
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::IconDragged
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
void GhostsPanel::IconDragged()
{
    protoType_.name_ = nameLineEdit_->text().ascii();
    protoType_.type_ = typeLineEdit_->text().ascii();
    protoType_.ghostType_ = symbolEditor_->GetGhostType();
    protoType_.symbol_ = symbolEditor_->GetSymbol();
    protoType_.nature_ = symbolEditor_->GetNature();
    protoType_.level_ = symbolEditor_->GetLevel();
    if( warningLabel_->text().isEmpty() )
    {
        Q3DragObject* drag = new gui::ValuedDragObject( &protoType_, this );
        drag->drag();
    }
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::UpdateWarning
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
void GhostsPanel::UpdateWarning()
{
    QString error;
    if( nameLineEdit_->text().isEmpty() )
        error += tr( "- Name not set.<br>" );
    if( typeLineEdit_->text().isEmpty() )
        error += tr( "- Type not set.<br>" );
    if( !symbolEditor_->IsLevelValid() )
        error += tr( "- Superior not set to a formation or an automat.<br>" );
    if( !error.isEmpty() )
        error = "<font color=\"#FF0000\">" + tr( "Error, missing informations:<br>" ) + error + "</font>";
    warningLabel_->setText( error );
}
