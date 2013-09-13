// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SearchListView.h"
#include "moc_ADN_SearchListView.cpp"
#include "ADN_GUI_ABC.h"
#include "ADN_ListView.h"
#include "ADN_Workspace.h"
#include "clients_gui/SearchLineEdit.h"

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SearchListView::ADN_SearchListView( const QString& objectName, ADN_GUI_ABC* gui, ADN_ListView& listView, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /* = -1*/, QWidget* parent /* = 0*/ )
    : QWidget( parent )
    , listView_( &listView )
    , lineEdit_( 0 )
    , subTab_  ( subTab )
{
    // Layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );

    // Create the list view.
    listView_->GetConnector().Connect( &reference );
    listView_->SetItemConnectors( connector );
    listView_->setObjectName( objectName );
    connect( listView_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    connect( gui, SIGNAL( ApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ), this, SLOT( OnApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ) );

    // Search box
    lineEdit_ = new gui::SearchLineEdit( objectName + "-search-field", this );

    connect( lineEdit_, SIGNAL( textChanged( const QString& ) ), listView_, SLOT( OnFilterChanged( const QString& ) ) );
    connect( listView_, SIGNAL( PurgeLineEdit() ), lineEdit_, SLOT( clear() ) );

    layout->addWidget( lineEdit_);
    layout->addWidget( listView_, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SearchListView::~ADN_SearchListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView::OnApplyFilterList
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
void ADN_SearchListView::OnApplyFilterList( const ADN_NavigationInfos::UsedBy& usedByInfos )
{
    if( usedByInfos.subTargetTab_ != subTab_ )
        return;

    lineEdit_->setReadOnly( true );
    lineEdit_->setText( usedByInfos.usingName_ );
    lineEdit_->setPaletteForegroundColor( Qt::darkGray );
    listView_->OnFilterChanged( usedByInfos.usersList_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView::GetListView
// Created: ABR 2013-02-07
// -----------------------------------------------------------------------------
ADN_ListView* ADN_SearchListView::GetListView() const
{
    return listView_;
}
