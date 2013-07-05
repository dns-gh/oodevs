// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DiffusionListDialog.h"
#include "moc_DiffusionListDialog.cpp"
#include "DiffusionListEditor.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "tools/Resolver.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog constructor
// Created: ABR 2011-05-10
// -----------------------------------------------------------------------------
DiffusionListDialog::DiffusionListDialog( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver< Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations, const kernel::ExtensionTypes& extensions, const char* name )
    : ModalDialog( parent, name )
    , extensions_( extensions )
    , contextMenuEntry_( true )
    , controllers_     ( controllers )
    , currentTeam_     ( controllers )
    , currentAgent_    ( controllers )
{
    // Init
    SubObjectName subObject( "DiffusionListDialog" );
    resize( 500, 700 );
    hide();

    // Editor
    editor_ = new DiffusionListEditor( this, controllers, agents, formations, extensions );

    // Buttons
    RichPushButton* generateButton = new RichPushButton( "generate", tr( "Generate" ) , this );
    RichPushButton* clearButton    = new RichPushButton( "clear", tr( "Clear" )    , this );
    RichPushButton* okButton       = new RichPushButton( "ok", tr( "Ok" )       , this );
    RichPushButton* cancelButton   = new RichPushButton( "cancel", tr( "Cancel" )   , this );
    okButton->setDefault( true );

    // Layouts
    QGridLayout* mainLayout = new QGridLayout( this, 3, 2, 0, 5 );
    mainLayout->setMargin( 5 );
    mainLayout->addWidget( editor_, 0, 0, 1, 2 );
    mainLayout->addWidget( generateButton, 1, 0 );
    mainLayout->addWidget( clearButton, 1, 1 );
    mainLayout->addWidget( okButton, 2, 0 );
    mainLayout->addWidget( cancelButton, 2, 1 );

    // Connections
    connect( okButton         , SIGNAL( clicked() ), SLOT( accept() ) );
    connect( cancelButton     , SIGNAL( clicked() ), SLOT( reject() ) );
    connect( clearButton      , SIGNAL( clicked() ), editor_, SLOT( OnClear() ) );
    connect( generateButton   , SIGNAL( clicked() ), editor_, SLOT( OnGenerate() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog destructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListDialog::~DiffusionListDialog()
{
    controllers_.Unregister( *this );
    currentTeam_ = 0;
    currentAgent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::SetSelected
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListDialog::SetCurrentTeam( const kernel::Entity_ABC& team )
{
    currentTeam_ = &team;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::SetCurrentAgent
// Created: ABR 2012-02-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::SetCurrentAgent( const kernel::Entity_ABC& agent )
{
    currentAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::SetContextMenuEntry
// Created: ABR 2012-02-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::SetContextMenuEntry( bool contextMenuEntry )
{
    contextMenuEntry_ = contextMenuEntry;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::NotifyContextMenu
// Created: ABR 2012-02-22
// -----------------------------------------------------------------------------
void DiffusionListDialog::NotifyContextMenu( const kernel::Team_ABC& team, ContextMenu& menu )
{
    if( !contextMenuEntry_ || team.GetId() == 0 || // no side team
        extensions_.GetNameByType( kernel::AttributeType::ETypeDiffusionList ).empty() ) // no diffusion list extension
        return;
    currentTeam_ = &team;
    currentAgent_ = 0;
    menu.InsertItem( "Helpers", tr( "Edit diffusion list" ), this, SLOT( Show() ), false, 11 );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::Show
// Created: ABR 2012-02-22
// -----------------------------------------------------------------------------
void DiffusionListDialog::Show()
{
    if( !currentTeam_ )
        return;
    setCaption( tr( "Diffusion list: %1" ).arg( currentTeam_->GetName() ) );
    editor_->Purge();
    if( !currentAgent_ )
        editor_->Fill( currentTeam_ );
    else
        editor_->Fill( currentTeam_, currentAgent_ );
    show();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::accept
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::accept()
{
    editor_->Commit();
    QDialog::accept();
    emit Accepted();
}
