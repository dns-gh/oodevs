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
#include "FeatureNameParser.h"
#include "LocationsLayer.h"
#include "LocationEditorBox.h"
#include "RichLineEdit.h"
#include "RichWidget.h"
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
    : RichToolBar( controllers, parent, "locationeditor", tr( "Location editor" ) )
    , controllers_( controllers )
    , converter_( converter )
    , view_( view )
    , layer_( layer )
    , bookmarksMenu_( 0 )
{
    locBox_ = new LocationEditorBox( controllers, converter );
    locBox_->AddParser( new FeatureNameParser( controllers ), tr( "Feature" ) );
    RichWidget< QToolButton >* gotoButton = new RichWidget< QToolButton >( "gotoButton", this );
    gotoButton->setIconSet( MAKE_PIXMAP( goto ) );
    gotoButton->setPopupDelay( 0 );
    gotoButton->setPopupMode( RichWidget< QToolButton >::MenuButtonPopup );
    bookmarksMenu_ = new kernel::ContextMenu( gotoButton );
    gotoButton->setPopup( bookmarksMenu_ );
    ClearBookmarks();
    QToolTip::add( gotoButton, tr( "Center on location" ) );
    paramsButton_ = new RichWidget< QToolButton >( "paramsButton", this );
    paramsButton_->setIconSet( MAKE_PIXMAP( special_point ) );
    QToolTip::add( paramsButton_, tr( "Set special point" ) );

    addWidget( locBox_ );
    addWidget( gotoButton );
    addWidget( paramsButton_ );

    connect( gotoButton, SIGNAL( clicked() ), SLOT( Goto() ) );
    connect( paramsButton_, SIGNAL( clicked() ), SLOT( AddParamPoint() ) );

    controllers_.Update( *this );
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
// Name: LocationEditorToolbar::AddParamPoint
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddParamPoint()
{
    geometry::Point2f point;
    if( GetPosition( point ) )
    {
        menuPoint_ = point;
        if( const LocationParser_ABC* parser = locBox_->GetCurrentParser() )
            CreateBookmark( parser->GetStringPosition( menuPoint_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::CreateBookmark
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::CreateBookmark()
{
    CreateBookmark( converter_.GetStringPosition( menuPoint_ ) );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::CreateBookmark
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
void LocationEditorToolbar::CreateBookmark( const std::string& defaultName )
{
    try
    {
        bool ok = false;
        const QString name = QInputDialog::getText( tr( "Create bookmark" ),
            tr( "Enter text to name the bookmark: " ), RichLineEdit::Normal, defaultName.c_str(), &ok, topLevelWidget() );
        if( !ok || name.isEmpty() )
            return;
        bookmarksMenu_->clear();
        bookmarks_.push_back( Bookmark( name.toStdString(), converter_.ConvertToMgrs( menuPoint_ ) ) );
        layer_.AddLocation( menuPoint_ );
        for( size_t i = 0; i < bookmarks_.size(); ++i )
        {
            int index = static_cast< int >( i );
            bookmarksMenu_->insertItem( bookmarks_[ i ].name_.c_str(), this, SLOT( GotoBookmark( int ) ), 0, index );
        }
        bookmarksMenu_->insertSeparator();
        bookmarksMenu_->insertItem( tr( "Clear bookmarks" ), this, SLOT( ClearBookmarks() ) );
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::GotoBookmark
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::GotoBookmark( int index )
{
    if( index >= 0 && index < static_cast< int >( bookmarks_.size() ) )
        try
        {
            view_.CenterOn( converter_.ConvertToXY( bookmarks_.at( index ).position_ ) );
        }
        catch( ... )
        {
            const int result = QMessageBox::critical( topLevelWidget(), tr( "Error" ),
                tr( "The bookmark location is not valid.\nDo you want to remove it?" ), QMessageBox::Yes, QMessageBox::No );
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
bool LocationEditorToolbar::GetPosition( geometry::Point2f& result ) const
{
    return locBox_->GetPosition( result );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::NotifyContextMenu
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void LocationEditorToolbar::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    menuPoint_ = point;
    menu.InsertItem( "Interface", tr( "Bookmark location" ), this, SLOT( CreateBookmark() ) );
}
