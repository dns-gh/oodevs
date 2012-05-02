// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::DiplomacyDialog_ABC */

#include "clients_gui_pch.h"
#include "DiplomacyDialog_ABC.h"
#include "moc_DiplomacyDialog_ABC.cpp"
#include "DiplomacyCell.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC constructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
DiplomacyDialog_ABC::DiplomacyDialog_ABC( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : ModalDialog( parent, "ChangeDiplomacyDialog" )
    , controllers_( controllers )
    , profile_    ( profile )
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setCaption( tools::translate( "gui::DiplomacyDialog_ABC", "Diplomacy" ) );
    setMaximumSize( 1024, 768);

    // Table
    table_ = new Q3Table( this ); // $$$$ ABR 2012-05-02: TODO Use QT4 Table instead
    table_->setSelectionMode( Q3Table::NoSelection );

    // Buttons
    QPushButton* okBtn     = new QPushButton( tools::translate( "gui::DiplomacyDialog_ABC", "Ok" ), this );
    QPushButton* cancelBtn = new QPushButton( tools::translate( "gui::DiplomacyDialog_ABC", "Cancel" ), this );
    okBtn->setDefault( true );
    okBtn->setMaximumWidth( 100 );
    cancelBtn->setMaximumWidth( 100 );

    // Layouts
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( this, 5, 5 );
    pMainLayout->addWidget( table_ );
    Q3HBoxLayout* pButtonLayout = new Q3HBoxLayout( pMainLayout, 5 );
    pButtonLayout->setAlignment( Qt::AlignCenter );
    pButtonLayout->addWidget( okBtn );
    pButtonLayout->addWidget( cancelBtn );

    // Connections
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okBtn    , SIGNAL( clicked() ), SLOT( Validate() ) );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC destructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
DiplomacyDialog_ABC::~DiplomacyDialog_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::Validate
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::Validate()
{
    for( unsigned int i = 0; i < teams_.size(); ++i )
        for( unsigned int j = 0; j < teams_.size(); ++j )
        {
            if( i == j )
                continue;
            DiplomacyCell* cell = static_cast< DiplomacyCell* >( table_->item( i, j ) );
            SetDiplomacy( *teams_[i], *teams_[j], cell->GetValue() );
        }
    hide();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::Reject
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::Reject()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::NotifyCreated
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::NotifyCreated( const kernel::Team_ABC& team )
{
    teams_.push_back( &team );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::NotifyDeleted
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::NotifyDeleted( const kernel::Team_ABC& team )
{
    T_Teams::iterator it = std::find( teams_.begin(), teams_.end(), &team );
    if( it != teams_.end() )
        teams_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::NotifyContextMenu
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( team ) )
        menu.InsertItem( "Command", tools::translate( "gui::DiplomacyDialog_ABC", "Diplomacy" ), this, SLOT( show() ) );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::showEvent
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::showEvent( QShowEvent* )
{
    UpdateTable();
    for( unsigned int i = 0; i < teams_.size(); ++i )
        for( unsigned int j = 0; j < teams_.size(); ++j )
        {
            if( i == j )
            {
                if( !table_->item( i, j ) )
                    table_->setItem( i, j, new Q3TableItem( table_, Q3TableItem::Never ) );
                table_->item( i, j )->setEnabled( false );
            }
            else
            {
                const kernel::Karma& diplomacy = teams_[i]->Get< Diplomacies_ABC >().GetDiplomacy( *teams_[j] );
                Q3TableItem* item = table_->item( i, j );
                if( !item )
                {
                    item = new DiplomacyCell( table_ );
                    table_->setItem( i, j, item );
                }
                item->setText( diplomacy == kernel::Karma::unknown_ ? kernel::Karma::neutral_.GetName() : diplomacy.GetName() ); // $$$$ SBO 2008-12-09:
            }
        }
    table_->adjustSize();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::UpdateTable
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::UpdateTable()
{
    table_->setNumCols( static_cast< int >( teams_.size() ) );
    table_->setNumRows( static_cast< int >( teams_.size() ) );
    int maxName = 0;
    for( unsigned i = 0; i < teams_.size(); ++i )
    {
        const QString name = teams_.at( i )->GetName();
        table_->verticalHeader  ()->setLabel( i, name );
        table_->horizontalHeader()->setLabel( i, name );
        table_->setRowStretchable( i, true );
        table_->setColumnStretchable( i, true );
        maxName = std::max< int >( name.size(), maxName );
    }
    int newWidth = teams_.size() * maxName * 10;
    int newHeight = teams_.size() * 40;
    table_->setMinimumSize( newWidth < maximumWidth() - 20 ? newWidth : maximumWidth() - 20, newHeight < maximumHeight() - 80 ? newHeight : maximumHeight() - 80 );
}
