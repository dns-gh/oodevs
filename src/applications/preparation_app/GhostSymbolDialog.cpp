// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GhostSymbolDialog.h"
#include "moc_GhostSymbolDialog.cpp"
#include "GhostSymbolEditor.h"
#include "clients_gui/NatureEditionCategory.h"
#include "clients_gui/NatureEditionWidget.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "preparation/AgentHierarchies.h"

// -----------------------------------------------------------------------------
// Name: GhostSymbolDialog constructor
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
GhostSymbolDialog::GhostSymbolDialog( QWidget* parent, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy )
    : QDialog( parent, "GhostSymbolDialog" )
    , controllers_   ( controllers )
    , selected_      ( controllers )
{
    // Initialize dialog
    setCaption( tr( "Phantom symbol editor" ) );

    // Symbol Editor
    QGridLayout* editorLayout = new QGridLayout();
    editorLayout->setAlignment( Qt::AlignTop );
    editorLayout->setMargin( 0 );
    editorLayout->setSpacing( 10 );
    editorLayout->setColStretch( 1, 1 );
    symbolEditor_ = new GhostSymbolEditor( editorLayout, 0, controllers, symbolsFactory, icons, colorStrategy );

    // Buttons
    QPushButton* okButton = new QPushButton( tr( "Ok" ) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    okButton->setDefault( TRUE );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    // Layouts
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );
    mainLayout->addLayout( editorLayout );
    mainLayout->addLayout( buttonLayout );

    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolDialog destructor
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
GhostSymbolDialog::~GhostSymbolDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolDialog::NotifyContextMenu
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolDialog::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    selected_ = &ghost;
    menu.InsertItem( "Helpers", tr( "Symbol" ), this, SLOT( Show() ), false, 10 );
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolDialog::Show
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolDialog::Show()
{
    if( !selected_ )
        return;
    symbolEditor_->Fill( *selected_ );
    exec();
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolDialog::Validate
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolDialog::Validate()
{
    if( !selected_ )
        return;
    selected_.ConstCast()->UpdateSymbol( symbolEditor_->GetLevel(), symbolEditor_->GetNature(), symbolEditor_->GetSymbol() );

    if( kernel::TacticalHierarchies* pTactical = selected_.ConstCast()->Retrieve< kernel::TacticalHierarchies >() )
    {
        static_cast< AgentHierarchies* >( pTactical )->UpdateSymbol( selected_->GetLevelSymbol(), selected_->GetSymbol() );
        controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = selected_->Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
    }
    selected_ = 0;
    accept();
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolDialog::Reject
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolDialog::Reject()
{
    selected_ = 0;
    reject();
}
