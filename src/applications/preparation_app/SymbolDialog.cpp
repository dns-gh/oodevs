// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SymbolDialog.h"
#include "moc_SymbolDialog.cpp"
#include "EntitySymbolEditor.h"
#include "clients_gui/NatureEditionCategory.h"
#include "clients_gui/NatureEditionWidget.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "preparation/Agent.h"
#include "preparation/GhostHierarchies.h"

// -----------------------------------------------------------------------------
// Name: SymbolDialog constructor
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
SymbolDialog::SymbolDialog( QWidget* parent, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory,
                            gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy )
    : QDialog( parent, "SymbolDialog" )
    , controllers_   ( controllers )
    , selected_      ( controllers )
    , symbolsFactory_( symbolsFactory )
{
    // Initialize dialog
    gui::SubObjectName subObject( "SymbolDialog" );
    setCaption( tr( "Symbol and hierarchy level editor" ) );
    setMinimumWidth( 400 );

    // Symbol Editor
    QGridLayout* editorLayout = new QGridLayout();
    editorLayout->setAlignment( Qt::AlignTop );
    editorLayout->setMargin( 0 );
    editorLayout->setSpacing( 10 );
    editorLayout->setColStretch( 1, 1 );
    symbolEditor_ = new EntitySymbolEditor( editorLayout, 0, controllers, symbolsFactory, icons, colorStrategy, QString(), 1 );

    // Buttons
    gui::RichPushButton* okButton = new gui::RichPushButton( "ok", tr( "Ok" ) );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ) );
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
// Name: SymbolDialog destructor
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
SymbolDialog::~SymbolDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SymbolDialog::NotifyContextMenu
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void SymbolDialog::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    selected_ = &ghost;
    symbolEditor_->SetDeep( 2 );
    menu.InsertItem( "Helpers", tr( "Symbol and hierarchy level" ), this, SLOT( Show() ), false, 10 );
}

// -----------------------------------------------------------------------------
// Name: SymbolDialog::NotifyContextMenu
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void SymbolDialog::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    selected_ = &agent;
    symbolEditor_->SetDeep( -1 );
    menu.InsertItem( "Helpers", tr( "Symbol and hierarchy level" ), this, SLOT( Show() ), false, 10 );
}

// -----------------------------------------------------------------------------
// Name: SymbolDialog::Show
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void SymbolDialog::Show()
{
    if( !selected_ )
        return;
    if( Agent* agent = dynamic_cast< Agent* >( selected_.ConstCast() ) )
        symbolEditor_->Fill( *selected_, agent->GetNature().c_str(), ENT_Tr::ConvertFromNatureLevel( agent->GetLevel() ).c_str() );
    else if( kernel::Ghost_ABC* ghost = dynamic_cast< kernel::Ghost_ABC* >( selected_.ConstCast() ) )
        symbolEditor_->Fill( *selected_, ghost->GetNature().c_str(), ghost->GetLevel().c_str() );
    else
        return;
    exec();
}

// -----------------------------------------------------------------------------
// Name: SymbolDialog::Validate
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void SymbolDialog::Validate()
{
    if( !selected_ )
        return;
    if( Agent* agent = dynamic_cast< Agent* >( selected_.ConstCast() ) )
    {
        // TODO symbol
        agent->SetLevel( ENT_Tr::ConvertToNatureLevel( symbolEditor_->GetLevel() ) );
        agent->SetSymbol( symbolEditor_->GetSymbol() );
        agent->SetNature( symbolEditor_->GetNature() );
        if( kernel::TacticalHierarchies* pTactical = agent->Retrieve< kernel::TacticalHierarchies >() )
        {
            if( pTactical->GetSuperior() )
                if( kernel::TacticalHierarchies* pSuperiorTactical = const_cast< kernel::Entity_ABC* >( pTactical->GetSuperior() )->Retrieve< kernel::TacticalHierarchies >() )
                {
                    pSuperiorTactical->UpdateSymbolUpward();
                    controllers_.controller_.Update( *pSuperiorTactical );
                }
            controllers_.controller_.Update( *pTactical );
        }
    }
    else if( kernel::Ghost_ABC* ghost = dynamic_cast< kernel::Ghost_ABC* >( selected_.ConstCast() ) )
    {
        ghost->UpdateSymbol( symbolEditor_->GetLevel(), symbolEditor_->GetNature(), symbolEditor_->GetSymbol() );
        if( kernel::TacticalHierarchies* pTactical = selected_.ConstCast()->Retrieve< kernel::TacticalHierarchies >() )
        {
            static_cast< GhostHierarchies* >( pTactical )->UpdateSymbol( ghost->GetLevelSymbol(), ghost->GetSymbol() );
            controllers_.controller_.Update( *pTactical );
            if( const kernel::CommunicationHierarchies* pCommunication = ghost->Retrieve< kernel::CommunicationHierarchies >() )
                controllers_.controller_.Update( *pCommunication );
            controllers_.controller_.Update( *pTactical );
        }
    }
    selected_ = 0;
    accept();
}

// -----------------------------------------------------------------------------
// Name: SymbolDialog::Reject
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void SymbolDialog::Reject()
{
    selected_ = 0;
    reject();
}
