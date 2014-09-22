// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyExclusiveListWidget.h"
#include "moc_LogisticSupplyExclusiveListWidget.cpp"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"

Q_DECLARE_METATYPE( const kernel::Automat_ABC* )

#define EntityRole ( Qt::UserRole )

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget constructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyExclusiveListWidget::LogisticSupplyExclusiveListWidget( kernel::Controllers& controllers,
                                                                      QWidget* parent,
                                                                      const QString& addLabel,
                                                                      const QString& removeLabel )
    : QListWidget( parent )
    , controllers_( controllers )
    , removeAction_( 0 )
    , addLabel_( addLabel )
    , removeLabel_( removeLabel )
{
    setSelectionMode( QAbstractItemView::SingleSelection );
    connect( this, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ),
        this, SLOT( OnSelectionChanged( QListWidgetItem*, QListWidgetItem* )  ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget destructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyExclusiveListWidget::~LogisticSupplyExclusiveListWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::contextMenuEvent
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::AddChoice( const kernel::Automat_ABC& automat )
{
    choice_.push_back( &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::contextMenuEvent
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::Clear()
{
    choice_.clear();
    choosen_.clear();
    clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::contextMenuEvent
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::contextMenuEvent( QContextMenuEvent* event )
{
    if( !event )
        return;
    QMenu* menu = new QMenu( this );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
    if( !choice_.empty() )
    {
        std::sort( choice_.begin(), choice_.end(), []( const kernel::Automat_ABC* lhs, const kernel::Automat_ABC* rhs ) {
            return lhs && rhs && lhs->GetName().localeAwareCompare( rhs->GetName() ) < 0;
        } );
        QMenu* subMenu = new QMenu( menu );
        menu->insertItem( addLabel_, subMenu );
        for( auto it = choice_.begin(); it != choice_.end(); ++it )
            subMenu->addAction( ( *it )->GetName() );
    }
    removeAction_ = currentItem() ? menu->addAction( removeLabel_ ) : 0;
    menu->popup( event->globalPos() );
    connect( menu, SIGNAL( triggered( QAction* ) ), this, SLOT( OnTriggeredAction( QAction* ) ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::OnSelectionChanged
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::OnSelectionChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
    QString cur, prev;
    if( current )
        cur = current->text();
    if( previous )
        prev = previous->text();
    emit SelectionChanged( cur, prev );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::OnTriggeredAction
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::OnTriggeredAction( QAction* action )
{
    if( !action )
        return;
    if( removeAction_ == action && currentItem() )
    {
        auto item = takeItem( currentIndex().row() );
        if( !item )
            return;
        auto automat = item->data( EntityRole ).value< const kernel::Automat_ABC* >();
        if( !automat )
            return;
        choice_.push_back( automat );
        auto it = std::find( choosen_.begin(), choosen_.end(), automat );
        if( it == choosen_.end() )
            return;
        choosen_.erase( it );
        emit ItemRemoved( automat->GetName() );
    }
    else
    {
        QString selected = action->text();
        auto it = std::find_if( choice_.begin(), choice_.end(), [&selected]( const kernel::Automat_ABC* automat ){ 
            return automat && automat->GetName() == selected;
        } );
        if( it == choice_.end() )
            return;

        QListWidgetItem* item = new QListWidgetItem( action->text() );
        item->setData( EntityRole, QVariant::fromValue( *it ) );
        addItem( item );

        choosen_.push_back( *it );
        choice_.erase( it );
        emit ItemAdded( selected );
        setCurrentRow( count() - 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::NotifyUpdated
// Created: ABR 2014-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::NotifyUpdated( const kernel::Automat_ABC& automat )
{
    for( int i = 0; i < count(); ++i )
        if( auto item = this->item( i ) )
            if( auto itemEntity = item->data( EntityRole ).value< const kernel::Automat_ABC* >() )
                if( itemEntity == &automat )
                    item->setText( itemEntity->GetName() );
}
