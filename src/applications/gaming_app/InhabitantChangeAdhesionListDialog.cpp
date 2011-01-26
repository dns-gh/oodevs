// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeAdhesionListDialog.h"
#include "moc_InhabitantChangeAdhesionListDialog.cpp"

#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Profile_ABC.h"

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeAdhesionListDialog::InhabitantChangeAdhesionListDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tr( "Change adhesion list" ) )
    , controllers_( controllers )
    , static_( staticModel )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , profile_( profile )
    , selected_( controllers )
{
    // Init dialog
    setCaption( tr( "Change adhesion list" ) );
    resize( 280, 280 );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 5 );
    mainLayout->setMargin( 5 );

    // Adhesion list informations


    // ok / cancel butons
    QHBoxLayout* buttonLayout = new QHBoxLayout( mainLayout );
    QPushButton* okButton     = new QPushButton( tr("Ok")    , this );
    QPushButton* cancelButton = new QPushButton( tr("Cancel"), this );
    buttonLayout->addWidget( okButton     );
    buttonLayout->addWidget( cancelButton );
    okButton->setDefault( TRUE );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    // Base
    selected_ = 0;
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeAdhesionListDialog::~InhabitantChangeAdhesionListDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAdhesionListDialog::Show()
{
    if( !selected_ )
        return;

    // Initialize();

    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAdhesionListDialog::Validate()
{
    if( ! selected_ )
        return;

    accept();

    // Throw Magic Action

    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAdhesionListDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAdhesionListDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAdhesionListDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAdhesionListDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tr( "Change adhesion list" ), this, SLOT( Show() ) );
    }
}
