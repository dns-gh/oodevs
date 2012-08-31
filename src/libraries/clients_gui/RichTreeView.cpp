// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTreeView.h"
#include "moc_RichTreeView.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTreeView constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
RichTreeView::RichTreeView( kernel::Controllers& controllers, QWidget* parent /*= 0*/ )
    : QTreeView( parent )
    , dataModel_( controllers, this )
    //, proxyModel_( this )
{
    setModel( &dataModel_ ); // $$$$ ABR 2012-08-17: To remove when proxy

    // $$$$ ABR 2012-08-17: Does work for now hide everything without any call to ApplyFilter
    //proxyModel_.setSourceModel( &dataModel_ );
    //proxyModel_.setFilterRole( StandardModel::FilterRole );
    //proxyModel_.setFilterRegExp( StandardModel::showValue_ );
    //setModel( &proxyModel_ );

    // $$$$ ABR 2012-08-17: TODO: May be use a custom proxy model for place pc in top of automat
    //proxyModel_->setDynamicSortFilter( true );
    //proxyModel_->setSortRole( Qt::UserRole );
}

// -----------------------------------------------------------------------------
// Name: RichTreeView destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
RichTreeView::~RichTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::Purge
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::Purge()
{
    dataModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SetCreationBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SetCreationBlocked( bool creationBlocked )
{
    creationBlocked_ = creationBlocked;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::IsCreationBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
bool RichTreeView::IsCreationBlocked() const
{
    return creationBlocked_;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SetContextMenuBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SetContextMenuBlocked( bool contextMenuBlocked )
{
    contextMenuBlocked_ = contextMenuBlocked;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::IsContextMenuBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
bool RichTreeView::IsContextMenuBlocked() const
{
    return contextMenuBlocked_;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SetDragAndDropBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SetDragAndDropBlocked( bool dragAndDropBlocked )
{
    dndBlocked_ = dragAndDropBlocked;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::IsDragAndDropBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
bool RichTreeView::IsDragAndDropBlocked() const
{
    return dndBlocked_;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::CreateFilters
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::CreateFilters( SearchListView_ABC& searchListView )
{
    // NOTHING
    // $$$$ ABR 2012-08-14: TODO for urban, nothing here, cf existing list view.
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SearchAndSelect
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SearchAndSelect( const QString& searchedText )
{
    // $$$$ ABR 2012-08-14: TODO, may be that should be on standard model
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SearchAndSelectNext
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SearchAndSelectNext()
{
    // $$$$ ABR 2012-08-14: TODO, may be that should be on standard model
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::MatchItem
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
bool RichTreeView::MatchItem() const
{
    // $$$$ ABR 2012-08-14: TODO, may be that should be on standard model
    return false;
}
