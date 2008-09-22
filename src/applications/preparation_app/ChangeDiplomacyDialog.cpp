// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ChangeDiplomacyDialog.h"
#include "moc_ChangeDiplomacyDialog.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Diplomacy.h"
#include "preparation/Diplomacies.h"
#include "DiplomacyCell.h"

using namespace kernel;
using namespace gui;

namespace
{
    DiplomacyCell* BuildDiplomacyCell( QTable* table, const QStringList& list )
    {
        DiplomacyCell* cell = new DiplomacyCell( table, list );
        cell->SetColor( Diplomacy::Friend() , QColor( 200, 200, 255 ) );
        cell->SetColor( Diplomacy::Enemy()  , QColor( 255, 200, 200 ) );
        cell->SetColor( Diplomacy::Neutral(), QColor( 200, 255, 200 ) );
        return cell;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent, Controllers& controllers )
    : QDialog( parent, "ChangeDiplomacyDialog", true )
    , controllers_( controllers )
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setCaption( tr( "Diplomacy" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );

    table_ = new QTable( this );
    table_->setSelectionMode( QTable::NoSelection );
    table_->setMinimumSize( 400, 200 );
    pMainLayout->addWidget( table_ );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    pButtonLayout->setAlignment( Qt::Right );
    QPushButton* okBtn     = new QPushButton( tr( "Ok" ), this );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), this );
    pButtonLayout->addWidget( okBtn );
    pButtonLayout->addWidget( cancelBtn );
    okBtn->setDefault( true );
    okBtn->setMaximumWidth( 100 );
    cancelBtn->setMaximumWidth( 100 );

    list_.append( Diplomacy::Friend().GetName() );
    list_.append( Diplomacy::Enemy().GetName() );
    list_.append( Diplomacy::Neutral().GetName() );

    connect( cancelBtn, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okBtn    , SIGNAL( clicked() ), SLOT( Validate() ) );

    controllers_.Register( *this );

    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::~ChangeDiplomacyDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Validate
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Validate()
{
    for( unsigned int i = 0; i < teams_.size(); ++i )
        for( unsigned int j = 0; j < teams_.size(); ++j )
        {
            if( i == j )
                continue;
            Diplomacies& diplomacies = const_cast< Diplomacies& >( teams_[i]->Get< Diplomacies >() );
            DiplomacyCell* cell = static_cast< DiplomacyCell* >( table_->item( i, j ) );
            diplomacies.SetDiplomacy( *teams_[j], cell->GetValue() );
        }
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Reject
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Reject()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyCreated( const Team_ABC& team )
{
    teams_.push_back( &team );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyDeleted( const Team_ABC& team )
{
    T_Teams::iterator it = std::find( teams_.begin(), teams_.end(), &team );
    if( it != teams_.end() )
        teams_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::UpdateTable
// Created: AGE 2007-06-28
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::UpdateTable()
{
    table_->setNumCols( teams_.size() );
    table_->setNumRows( teams_.size() );
    for( unsigned i = 0; i < teams_.size(); ++i )
    {
        const QString name = teams_.at( i )->GetName();
        table_->verticalHeader  ()->setLabel( i, name );
        table_->horizontalHeader()->setLabel( i, name );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyContextMenu
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyContextMenu( const Team_ABC& , ContextMenu& menu )
{
    menu.InsertItem( "Command", tr( "Diplomacy" ), this, SLOT( show() ) );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::showEvent
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::showEvent( QShowEvent* )
{
    UpdateTable();

    for( unsigned int i = 0; i < teams_.size(); ++i )
        for( unsigned int j = 0; j < teams_.size(); ++j )
        {
            if( i == j )
            {
                if( !table_->item( i, j ) )
                    table_->setItem( i, j, new QTableItem( table_, QTableItem::Never ) );
                table_->item( i, j )->setEnabled( false );
            }
            else
            {
                const Diplomacy& diplomacy = const_cast< Diplomacies& >( teams_[i]->Get< Diplomacies >() ).GetDiplomacy( *teams_[j] );
                QTableItem* item = table_->item( i, j );
                if( !item )
                {
                    item = BuildDiplomacyCell( table_, list_ );
                    table_->setItem( i, j, item );
                }
                item->setText( diplomacy.GetName() );
            }
        }
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::sizeHint
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
QSize ChangeDiplomacyDialog::sizeHint()
{
    const QRect rect = table_->cellGeometry( 1, 1 );
    return QSize( ( teams_.size() + 1 ) * rect.width(), teams_.size() * rect.height() + 50 );
}
