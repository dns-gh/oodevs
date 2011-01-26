// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantExtractCrowdDialog.h"
#include "moc_InhabitantExtractCrowdDialog.cpp"

#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Profile_ABC.h"

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantExtractCrowdDialog::InhabitantExtractCrowdDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tr( "Extract crowd" ) )
    , controllers_( controllers )
    , static_( staticModel )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , profile_( profile )
    , selected_( controllers )
{
    // Init dialog
    setCaption( tr( "Extract crowd" ) );
    resize( 280, 280 );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 5 );
    mainLayout->setMargin( 5 );

    // Crowd informations


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
// Name: InhabitantExtractCrowdDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantExtractCrowdDialog::~InhabitantExtractCrowdDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::Show()
{
    if( !selected_ )
        return;

    // Initialize();

    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::Validate()
{
    if( ! selected_ )
        return;

    accept();

    // Throw Magic Action

    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tr( "Extract crowd" ), this, SLOT( Show() ) );
    }
}
