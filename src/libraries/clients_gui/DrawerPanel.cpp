// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerPanel.h"
#include "moc_DrawerPanel.cpp"
#include "xeumeuleu/xml.h"
#include <qtoolbox.h>
#include "DrawerCategory.h"
#include "DrawerLayer.h"
#include "ColorButton.h"
#include "DrawerFactory.h"
#include "DrawerModel.h"
#include "clients_kernel/Controllers.h"
#include "svgl/TextRenderer.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerPanel constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::DrawerPanel( QWidget* parent, DrawerLayer& layer, kernel::GlTools_ABC& tools, kernel::Controllers& controllers )
    : QVBox( parent, "Drawer" )
    , controllers_( controllers )
    , layer_( layer )
    , renderer_( new svg::TextRenderer() )
    , factory_( *new DrawerFactory( this, tools, *renderer_, controllers_ ) )
    , model_( *new DrawerModel( factory_ ) )
{
    QHBox* box = new QHBox( this );
    box->layout()->setAlignment( Qt::AlignCenter );
    box->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    box->setBackgroundMode( Qt::PaletteButton );
    box->setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

    QToolButton* btn = new QToolButton( box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( open ) );
    QToolTip::add( btn, tr( "Load drawings file" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Open() ) );

    btn = new QToolButton( box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( save ) );
    QToolTip::add( btn, tr( "Save drawings to file" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Save() ) );

    btn = new QToolButton( box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( cross ) );
    btn->setFixedSize( 25, 25 );
    QToolTip::add( btn, tr( "Clear drawings" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Clear() ) );

    color_ = new ColorButton( box, "" );
    toolBox_ = new QToolBox( this );
    toolBox_->setMargin( 0 );
    toolBox_->setBackgroundColor( Qt::white );
    
    controllers_.Register( *this );

    model_.Load( "DrawingTemplates.xml" ); // $$$$ SBO 2007-03-22: 
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::~DrawerPanel()
{
    controllers_.Unregister( *this );
    // $$$$ SBO 2007-03-22: renderer ?
    delete &factory_;
    delete &model_;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::NotifyCreated
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerPanel::NotifyCreated( const DrawerCategory& category )
{
    DrawerCategory* cat = const_cast< DrawerCategory* >( &category );
    connect( cat, SIGNAL( Selected( DrawerStyle& ) ), SLOT( OnSelect( DrawerStyle& ) ) );
    int id = toolBox_->addItem( cat, MAKE_PIXMAP( drawings ), category.GetName() );
    toolBox_->setItemToolTip ( id, category.GetDescription() );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::OnSelect
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerPanel::OnSelect( DrawerStyle& style )
{
    color_->Commit();
    layer_.StartShape( style, color_->GetColor() );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::hideEvent
// Created: AGE 2006-09-07
// -----------------------------------------------------------------------------
void DrawerPanel::hideEvent( QHideEvent* )
{
    emit Closed();
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Open
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerPanel::Open()
{
    QString filename = QFileDialog::getOpenFileName( "", tr( "Drawings (*.xml)" ), this, 0, tr( "Load drawings file" ) );
    if( filename.isEmpty() )
        return;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    try
    {
        layer_.Load( filename.ascii(), model_ );
    }
    catch( xml::exception& )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "'%1' is not a valid drawings file." ).arg( filename ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Save
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerPanel::Save()
{
    QString filename = QFileDialog::getSaveFileName( "", tr( "Drawings (*.xml)" ), this, 0, tr( "Save drawings to file" ) );
    if( filename.isEmpty() )
        return;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" ); 
    if( !filename.endsWith( ".xml" ) )
        filename.append( ".xml" );
    try
    {
        layer_.Save( filename.ascii() );
    }
    catch( xml::exception& )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Unable to save drawings to file '%1'. \nPlease check access permissions or write protection." ).arg( filename ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Clear
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerPanel::Clear()
{
    layer_.Clear();
}
