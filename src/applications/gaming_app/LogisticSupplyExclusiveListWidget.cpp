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

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget constructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyExclusiveListWidget::LogisticSupplyExclusiveListWidget( QWidget* parent, const QString& addLabel, const QString& removeLabel )
    : QListWidget( parent )
    , removeAction_( 0 )
    , addLabel_( addLabel )
    , removeLabel_( removeLabel )
{
    setSelectionMode( QAbstractItemView::SingleSelection );
    connect( this, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ),
        this, SLOT( OnSelectionChanged( QListWidgetItem*, QListWidgetItem* )  ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget destructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyExclusiveListWidget::~LogisticSupplyExclusiveListWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::contextMenuEvent
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::SetChoice( const QStringList& choice )
{
    Clear();
    choice_ = choice;
    choice_.sort();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::contextMenuEvent
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyExclusiveListWidget::GetItems( QStringList& items )
{
    items = choosen_;
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
        QMenu* subMenu = new QMenu( menu );
        menu->insertItem( addLabel_, subMenu );
        for( int i = 0; i < choice_.size(); ++i )
            subMenu->addAction( choice_[i] );
    }
    removeAction_ = 0;
    if( currentItem() )
        removeAction_ = menu->addAction( removeLabel_ );
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
        QString current = currentItem()->text();
        takeItem( currentIndex().row() );
        choosen_.remove( current );
        choice_.append( current );
        choice_.sort();
        emit ItemRemoved( current );
    }
    else
    {
        QString selected = action->text();
        addItem( selected );
        choice_.remove( selected );
        choosen_.append( selected );
        choosen_.sort();
        emit ItemAdded( selected );
        setCurrentRow( count() - 1 );
    }
}
