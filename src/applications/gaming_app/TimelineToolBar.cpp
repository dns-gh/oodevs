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
#include "clients_gui/FileDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( QWidget* parent, const tools::ExerciseConfig& config, bool isMain, const QStringList& activeFilters )
    : QToolBar( parent )
    , config_( config )
{
    addAction( qApp->style()->standardIcon( QStyle::SP_BrowserReload ), tr( "Center the view on the simulation time" ), this, SLOT( OnCenterView() ) );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/eye.png" ) ), tr( "Edit filters" ), this, SLOT( OnFilterSelection() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOpenButton ), tr( "Load actions file" ), this, SLOT( OnLoadOrderFile() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save actions in active timeline to file" ), this, SLOT( OnSaveOrderFile() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOkButton ), tr( "Create a new view" ), this, SLOT( OnAddNewFilteredView() ) );
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

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnLoadOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnLoadOrderFile()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load actions file" ), config_.BuildExerciseChildFile( "orders.ord" ), tr( "Actions files (*.ord)" ) );
    if( !filename.IsEmpty() && filename.Exists() && filename.IsRegularFile() )
        emit LoadOrderFileRequest( filename );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnSaveOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnSaveOrderFile()
{
    tools::Path filename = gui::FileDialog::getSaveFileName( this, tr( "Save actions in active timeline to file" ), config_.BuildExerciseChildFile( "orders" ), tr( "Actions files (*.ord)" ) );
    if( filename.IsEmpty() )
        return;
    if( filename.Extension() != ".ord" )
        filename.ReplaceExtension( ".ord" );
    emit SaveOrderFileRequest( filename );
}
