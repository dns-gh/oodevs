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
#include "XyParser.h"
#include "FeatureNameParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "resources.h"
#include <qinputdialog.h>
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
    , view_( view )
    , layer_( layer )
    , parameters_( 0 )
    , bookmarksMenu_( 0 )
{
    // $$$$ AGE 2008-05-29: externaliser
    parsers_.push_back( new UtmParser( controllers, converter ) );
    parsers_.push_back( new XyParser() );
    parsers_.push_back( new FeatureNameParser( controllers ) );

    setLabel( tr( "Location editor" ) );
    new QLabel( tr( "Location: " ), this );
    utm_ = new QLineEdit( this );
    QToolTip::add( utm_, tr( "Enter UTM coordinate, local coordinate or feature name" ) );
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

    subMenu_ = new QPopupMenu();
    list_ = new QListBox( subMenu_ );
    list_->setColumnMode( QListBox::Variable );
    list_->setRowMode( QListBox::Variable );
    list_->setFrameStyle( QFrame::NoFrame );
    subMenu_->insertItem( list_ );
    subMenu_->hide();

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
// Name: LocationEditorToolbar::AddParser
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddParser( std::auto_ptr< LocationParser_ABC > parser )
{
    parsers_.push_back( parser.release() );
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
    subMenu_->hide();
    if( utm_->text().isEmpty() )
        return false;
    QString hint;
    T_Parsers::const_iterator it = std::find_if( parsers_.begin(), parsers_.end(),
        boost::bind( &LocationParser_ABC::Parse, _1, utm_->text(), boost::ref( result ), boost::ref( hint ) ) );

    SetAspect( hint, it == parsers_.end() );
    return it != parsers_.end();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::SetAspect
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void LocationEditorToolbar::SetAspect( const QString& hint, bool red )
{
    if( !hint.isEmpty() && hint != utm_->text() )
    {
        list_->clear();
        list_->insertItem( hint );
        list_->setMinimumSize( 110, 12 );
        const QPoint topLeft = utm_->mapToGlobal( QPoint( 0, 0 ) );
        subMenu_->popup( QPoint( topLeft.x(), topLeft.y() + utm_->height() ) );
        utm_->setFocus();
    }
    if( red )
        utm_->setPaletteBackgroundColor( Qt::red.light( 120 ) );
    else
        utm_->unsetPalette();
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
