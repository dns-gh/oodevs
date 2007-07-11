// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LocationEditorToolbar.h"
#include "moc_LocationEditorToolbar.cpp"
#include "ParametersLayer.h"
#include "View_ABC.h"
#include "LocationsLayer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar constructor
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
LocationEditorToolbar::LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view, LocationsLayer& layer )
    : QToolBar( parent, "location editor" )
    , controllers_( controllers )
    , converter_( converter )
    , view_( view )
    , layer_( layer )
    , parameters_( 0 )
    , bookmarksMenu_( 0 )
{
    setLabel( tr( "Location editor" ) );
    new QLabel( tr( "Location: " ), this );
    utm_ = new QLineEdit( this );
    QToolTip::add( utm_, tr( "Enter UTM coordinate" ) );
    utm_->setMaxLength( 20 );
    utm_->setFixedWidth( 110 );
    gotoButton_ = new QToolButton( this );
    gotoButton_->setIconSet( MAKE_PIXMAP( goto ) );
    gotoButton_->setPopupDelay( 0 );
    bookmarksMenu_ = new QPopupMenu( gotoButton_ );
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

    connect( utm_, SIGNAL( returnPressed() ), SLOT( Goto() ) );
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
    if( utm_->text().isEmpty() )
        return;
    try
    {
        view_.CenterOn( GetPosition() );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::AddPoint
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddPoint()
{
    if( !parameters_ || utm_->text().isEmpty() )
        return;
    try
    {
        parameters_->AddPoint( GetPosition() );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::AddParamPoint
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddParamPoint()
{
    if( utm_->text().isEmpty() )
        return;
    try
    {
        controllers_.actions_.ContextMenu( GetPosition(), mapToGlobal( paramsButton_->geometry().bottomLeft() ) );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::Bookmark
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::Bookmark()
{
    try
    {
        std::string utm = converter_.ConvertToMgrs( menuPoint_ );
        bookmarksMenu_->clear();
        bookmarks_.push_back( utm );
        layer_.AddLocation( menuPoint_ );
        unsigned int i = 0;
        for( CIT_Bookmarks it = bookmarks_.begin(); it != bookmarks_.end(); ++it, ++i )
            bookmarksMenu_->insertItem( it->c_str(), this, SLOT( GotoBookmark( int ) ), 0, i );
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
            view_.CenterOn( converter_.ConvertToXY( bookmarks_[index] ) );
        }
        catch( ... )
        {
            // $$$$ SBO 2007-03-28: remove invalid location from list?
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

namespace
{
    geometry::Point2f Parse( std::string coord )
    {
        std::replace( coord.begin(), coord.end(), ',', ' ' );
        std::replace( coord.begin(), coord.end(), ';', ' ' );
        std::replace( coord.begin(), coord.end(), '[', ' ' );
        std::replace( coord.begin(), coord.end(), ']', ' ' );
        std::replace( coord.begin(), coord.end(), '(', ' ' );
        std::replace( coord.begin(), coord.end(), ')', ' ' );

        std::stringstream str( coord );
        float x, y;
        str >> x >> y;
        if( ! str )
            throw std::runtime_error( "" );
        return geometry::Point2f( x, y );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::GetPosition
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
geometry::Point2f LocationEditorToolbar::GetPosition() const
{
    const std::string coord( utm_->text().ascii() );
    try {
        return converter_.ConvertToXY( coord );
    } catch( ... ) {}

    try {
        return Parse( coord );
    } catch( ... )
    {
        QMessageBox::critical( topLevelWidget(), tr( "Error" ), "'" + utm_->text() + "'" + tr( " is not a valid coordinate." ) );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::NotifyContextMenu
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    menuPoint_ = point;
    menu.InsertItem( "Helpers", tr( "Bookmark location" ), this, SLOT( Bookmark() ) );
}
