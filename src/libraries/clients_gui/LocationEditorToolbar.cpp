// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::LocationEditorToolbar */

#include "clients_gui_pch.h"
#include "LocationEditorToolbar.h"
#include "moc_LocationEditorToolbar.cpp"
#include "ParametersLayer.h"
#include "View_ABC.h"
#include "LocationsLayer.h"
#include "LocationParser_ABC.h"
#include "UtmParser.h"
#include "XYParser.h"
#include "Wgs84DdParser.h"
#include "Wgs84DmsParser.h"
#include "FeatureNameParser.h"
#include "LocationEditorBox.h"
#include "LocationParsers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "resources.h"
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar constructor
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
LocationEditorToolbar::LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view, LocationsLayer& layer )
    : QToolBar( parent, "location editor" )
    , controllers_( controllers )
    , converter_( converter )
    , featureNameParser_( controllers )
    , view_( view )
    , layer_( layer )
    , parameters_( 0 )
    , bookmarksMenu_( 0 )
{
    setObjectName( "locEditToolBar" );
    setWindowTitle( tr( "Location editor" ) );
    locBox_ = new LocationEditorBox( this, controllers, converter );
    locBox_->AddParser( featureNameParser_, tr( "Feature" ) );
    gotoButton_ = new QToolButton( this );
    gotoButton_->setIconSet( MAKE_PIXMAP( goto ) );
    gotoButton_->setPopupDelay( 0 );
    gotoButton_->setPopupMode( QToolButton::MenuButtonPopup );
    bookmarksMenu_ = new Q3PopupMenu( gotoButton_ );
    gotoButton_->setPopup( bookmarksMenu_ );
    ClearBookmarks();
    QToolTip::add( gotoButton_, tr( "Center on location" ) );
    okButton_ = new QToolButton( this );
    okButton_->setIconSet( MAKE_PIXMAP( add_point ) );
    QToolTip::add( okButton_, tr( "Add point to current location" ) );
    okButton_->hide();
    paramsButton_ = new QToolButton( this );
    paramsButton_->setIconSet( MAKE_PIXMAP( special_point ) );
    QToolTip::add( paramsButton_, tr( "Set special point" ) );

    addWidget( locBox_ );
    addWidget( gotoButton_ );
    addWidget( okButton_ );
    addWidget( paramsButton_ );

    connect( gotoButton_, SIGNAL( clicked() ), SLOT( Goto() ) );
    connect( okButton_, SIGNAL( clicked() ), SLOT( AddPoint() ) );
    connect( paramsButton_, SIGNAL( clicked() ), SLOT( AddParamPoint() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar destructor
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
LocationEditorToolbar::~LocationEditorToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::StartEdit
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::StartEdit( ParametersLayer& parameters )
{
    show();
    parameters_ = &parameters;
    okButton_->show();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::EndEdit
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::EndEdit()
{
    parameters_ = 0;
    okButton_->hide();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::Goto
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::Goto()
{
    geometry::Point2f point;
    if( GetPosition( point ) )
        view_.CenterOn( point );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::AddPoint
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddPoint()
{
    geometry::Point2f point;
    if( parameters_  && GetPosition( point ) )
        parameters_->AddPoint( point );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::AddParamPoint
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddParamPoint()
{
    geometry::Point2f point;
    if( GetPosition( point ) )
        controllers_.actions_.ContextMenu( point, mapToGlobal( paramsButton_->geometry().bottomLeft() ) );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::CreateBookmark
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::CreateBookmark()
{
    try
    {
        const std::string utm = converter_.ConvertToMgrs( menuPoint_ );
        bool ok = false;
        const QString name = QInputDialog::getText( tr( "Create bookmark" ), tr( "Enter text to name the bookmark: " ), QLineEdit::Normal, utm.c_str(), &ok, topLevelWidget() );
        if( !ok || name.isEmpty() )
            return;
        bookmarksMenu_->clear();
        bookmarks_.push_back( Bookmark( name.ascii(), utm ) );
        layer_.AddLocation( menuPoint_ );
        unsigned int i = 0;
        for( CIT_Bookmarks it = bookmarks_.begin(); it != bookmarks_.end(); ++it, ++i )
            bookmarksMenu_->insertItem( it->name_.c_str(), this, SLOT( GotoBookmark( int ) ), 0, i );
        bookmarksMenu_->insertSeparator();
        bookmarksMenu_->insertItem( tr( "Clear bookmarks" ), this, SLOT( ClearBookmarks() ) );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::GotoBookmark
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::GotoBookmark( int index )
{
    if( index >= 0 && index < int( bookmarks_.size() ) )
        try
        {
            view_.CenterOn( converter_.ConvertToXY( bookmarks_.at( index ).position_ ) );
        }
        catch( ... )
        {
            const int result = QMessageBox::critical( topLevelWidget(), tr( "Error" ), tr( "The bookmark location is not valid.\nDo you want to remove it?" ), QMessageBox::Yes, QMessageBox::No );
            if( result == QMessageBox::Yes )
                bookmarks_.erase( bookmarks_.begin() + index );
        }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::ClearBookmarks
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::ClearBookmarks()
{
    bookmarks_.clear();
    layer_.Reset();
    bookmarksMenu_->clear();
    bookmarksMenu_->insertItem( tr( "No bookmark defined" ) );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::GetPosition
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool LocationEditorToolbar::GetPosition( geometry::Point2f& result )
{
    bool parseSucceed = locBox_->GetPosition( result );
    return parseSucceed;
}
// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::NotifyContextMenu
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    menuPoint_ = point;
    menu.InsertItem( "Helpers", tr( "Bookmark location" ), this, SLOT( CreateBookmark() ) );
}
