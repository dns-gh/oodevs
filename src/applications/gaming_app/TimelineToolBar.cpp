// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineToolBar.h"
#include "moc_TimelineToolBar.cpp"
#include "clients_gui/ImageWrapper.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( QWidget* parent, bool isMain, const QStringList& activeFilters )
    : QToolBar( parent )
{
    addAction( qApp->style()->standardIcon( QStyle::SP_DirHomeIcon ), tr( "Center the view on the simulation time" ), this, SLOT( OnCenterView() ) );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/eye.png" ) ), tr( "Edit filters" ), this, SLOT( OnFilterSelection() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Create a new view" ), this, SLOT( OnAddNewFilteredView() ) );
    if( !isMain )
        addAction( qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), tr( "Remove current view" ), this, SLOT( OnRemoveCurrentFilteredView() ) );

    filtersMenu_ = new QMenu( this );
    AddFilter( "orders", tr( "Orders" ), activeFilters );
    AddFilter( "reports", tr( "Reports" ), activeFilters );
    AddFilter( "supervisor", tr( "Supervisor actions" ), activeFilters );
    AddFilter( "multimedia", tr( "Multimedia" ), activeFilters );
    AddFilter( "tasks",      tr( "Tasks" ), activeFilters );
    filtersMenu_->addSeparator();
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::~TimelineToolBar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::AddFilter
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::AddFilter( const QString& filter, const QString& displayName, const QStringList& filters )
{
    AddFilter( filter, displayName, filters.contains( "all" ) || filters.contains( filter ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::AddFilter
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::AddFilter( const QString& filter, const QString& displayName, bool isActive )
{
    Filter newFilter;
    newFilter.filter_ = filter;
    newFilter.displayName_ = displayName;
    newFilter.isActive_ = isActive;
    filters_.push_back( newFilter );
    QAction* action = filtersMenu_->addAction( displayName );
    action->setCheckable( true );
    action->setChecked( isActive );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetActiveFilters
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
QStringList TimelineToolBar::GetActiveFilters()
{
    QStringList activeFilters;
    for( auto it = filters_.begin(); it != filters_.end(); ++it )
    {
        if( it->isActive_ )
            activeFilters << it->filter_;
    }
    return activeFilters;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnCenterView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnCenterView()
{
    // Center the view !
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnFilterSelection
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnFilterSelection()
{
    // $$$$ ABR 2013-05-28: Use a better trick than a QMenu to allow multiple selection in only one click
    filtersMenu_->hideTearOffMenu();
    QAction* resultingAction = filtersMenu_->exec( QCursor::pos() );
    if( !resultingAction )
        return;
    for( auto it = filters_.begin(); it != filters_.end(); ++it )
        if( it->displayName_ == resultingAction->text() )
            it->isActive_ = resultingAction->isChecked();
    emit FilterSelectionChanged( GetActiveFilters() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnAddNewFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnAddNewFilteredView()
{
    emit AddNewFilteredView( GetActiveFilters() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnRemoveCurrentFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnRemoveCurrentFilteredView()
{
    emit RemoveCurrentFilteredView();
}
