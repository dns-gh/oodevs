// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GUI_ABC.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 18:29 $
// $Revision: 3 $
// $Workfile: ADN_GUI_ABC.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GUI_ABC.h"
#include "moc_ADN_GUI_ABC.cpp"
#include "ADN_NavigationInfos.h"
#include "ADN_ListView.h"
#include "ADN_Tr.h"
#include <QtGui/qsizepolicy.h>

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC constructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
ADN_GUI_ABC::ADN_GUI_ABC( E_WorkspaceElements workspaceElement )
    : pMainWidget_ ( 0 )
    , pListView_( 0 )
    , strClassName_( ADN_Tr::ConvertFromWorkspaceElement( workspaceElement, ADN_Tr::eToSim ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC destructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
ADN_GUI_ABC::~ADN_GUI_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::CreateMainWidget
// Created: ABR 2012-01-20
// -----------------------------------------------------------------------------
QWidget* ADN_GUI_ABC::CreateScrollArea( const char* objectName, QWidget& content, QWidget* list /* = 0*/, bool paintSplitter /* = true*/, bool paintBackground /* = false*/, bool showFrameBorder /* = true*/, int margin /* = 10*/, int spacing /* = 10*/ )
{
    // Content area
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setName( "ScrollArea");
    scrollArea->setWidget( &content );
    scrollArea->setWidgetResizable( true );

    // Backup color because splitter's background color applies to his children.
    QColor color = scrollArea->backgroundColor();

    // MainWidget
    QWidget* mainWidget = new QWidget( 0 );
    QHBoxLayout* pMainLayout = new QHBoxLayout( mainWidget );
    pMainLayout->setMargin( margin );
    pMainLayout->setSpacing( spacing );

    if( list == 0 )
        pMainLayout->addWidget( scrollArea, 1 );
    else
    {
        QSplitter* splitter = new QSplitter();
        pMainLayout->addWidget( splitter, 1 );
        splitter->setOpaqueResize( false );
        if( paintSplitter )
            splitter->setBackgroundColor( Qt::white );
        splitter->addWidget( list );
        splitter->addWidget( scrollArea );

        splitter->setCollapsible( list, false );
        splitter->setCollapsible( scrollArea, false );
        list->setMinimumWidth( 100 );
        list->setMaximumWidth( 350 );
    }

    scrollArea->setBackgroundColor( ( paintBackground ) ? Qt::white : color );
    if( !showFrameBorder )
        scrollArea->setFrameShape( QFrame::NoFrame );

    mainWidget->setObjectName( objectName );

    return mainWidget;
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::ApplyFilter
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
void ADN_GUI_ABC::ApplyFilter( const ADN_NavigationInfos::UsedBy& usedByInfos )
{
    emit ApplyFilterList( usedByInfos );
    if( usedByInfos.subTargetTab_ != -1 )
        ChangeCurrentSubTab( usedByInfos.subTargetTab_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::ClearFilters
// Created: JSR 2012-12-14
// -----------------------------------------------------------------------------
void ADN_GUI_ABC::ClearFilters()
{
    if( pListView_ )
        pListView_->ClearFilters();
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::SelectItem
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
bool ADN_GUI_ABC::SelectItem( const QString& name )
{
    if( pListView_ )
        return pListView_->SetCurrentItem( name );
    return false;
}

// -----------------------------------------------------------------------------
// ADN_Tabbed_GUI_ABC
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Tabbed_GUI_ABC::ChangeCurrentSubTab
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_Tabbed_GUI_ABC::ChangeCurrentSubTab( int subTab )
{
    if( subTab >= 0 && subTab < pTabWidget_->count() )
        pTabWidget_->setCurrentPage( subTab );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tabbed_GUI_ABC::SelectItem
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
bool ADN_Tabbed_GUI_ABC::SelectItem( const QString& name )
{
    if( !( pTabWidget_ && pTabWidget_->currentIndex() >= 0 && pTabWidget_->currentIndex() < static_cast< int >( vListViews_.size() )
        && vListViews_[ pTabWidget_->currentIndex() ] ) )
        return false;
    return vListViews_[ pTabWidget_->currentIndex() ]->SetCurrentItem( name );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tabbed_GUI_ABC::FindSubTabAndSelectItem
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void ADN_Tabbed_GUI_ABC::FindSubTabAndSelectItem( const QString& name )
{
    if( !pTabWidget_ )
        return;
    for( int i = 0; i < static_cast< int >( vListViews_.size() ); ++i )
    {
        if( vListViews_[ i ]->FindItem( name ) != 0 )
        {
            ChangeCurrentSubTab( i );
            vListViews_[ i ]->SetCurrentItem( name );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::AddListView
// Created: ABR 2013-02-07
// -----------------------------------------------------------------------------
void ADN_GUI_ABC::AddListView( ADN_ListView* listView )
{
    assert( listView != 0 );
    pListView_ = listView;
}

// -----------------------------------------------------------------------------
// Name: ADN_Tabbed_GUI_ABC::AddListView
// Created: ABR 2013-02-07
// -----------------------------------------------------------------------------
void ADN_Tabbed_GUI_ABC::AddListView( ADN_ListView* listView )
{
    assert( listView != 0 );
    vListViews_.push_back( listView );
}
