// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::DrawerPanel */

#include "clients_gui_pch.h"
#include "DrawerPanel.h"
#include "moc_DrawerPanel.cpp"
#include "ParametersLayer.h"
#include "ColorButton.h"
#include "DrawerModel.h"
#include "Drawing_ABC.h"
#include "DrawingCategory.h"
#include "DrawingTemplate.h"
#include "DrawingCategoryItem.h"
#include "clients_kernel/Controllers.h"
#include "resources.h"
#include <xeumeuleu/xml.h>
#include <qtoolbox.h>

#pragma warning( disable : 4355 )

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerPanel constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::DrawerPanel( QWidget* parent, PanelStack_ABC& panel, ParametersLayer& layer, kernel::Controllers& controllers, DrawerModel& model )
    : InfoPanel_ABC   ( parent, panel, tr( "Drawings" ) )
    , controllers_    ( controllers )
    , layer_          ( layer )
    , model_          ( model )
    , selectedStyle_  ( 0 )
    , selectedDrawing_( controllers )
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
    connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChange( const QColor& ) ) );

    btn = new QToolButton( box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( pencil ) );
    btn->setFixedSize( 25, 25 );
    QToolTip::add( btn, tr( "Start drawing" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( StartDrawing() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::~DrawerPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::NotifyCreated
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerPanel::NotifyCreated( const DrawingCategory& category )
{
    DrawingCategoryItem* item = new DrawingCategoryItem( this, category );
    connect( item, SIGNAL( Selected( const DrawingTemplate& ) ), SLOT( OnSelect( const DrawingTemplate& ) ) );
    const int id = toolBox_->addItem( item, MAKE_PIXMAP( drawings ), category.GetName() );
    toolBox_->setItemToolTip( id, category.GetDescription() );
    categories_[ &category ] = item;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::NotifyDeleted
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void DrawerPanel::NotifyDeleted( const DrawingCategory& category )
{
    T_CategoryItems::const_iterator it = categories_.find( &category );
    if( it != categories_.end() )
    {
        toolBox_->removeItem( it->second );
        delete it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::NotifyUpdated
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void DrawerPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::NotifySelected
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void DrawerPanel::NotifySelected( const Drawing_ABC* drawing )
{
    selectedDrawing_ = drawing;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::OnSelect
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerPanel::OnSelect( const DrawingTemplate& style )
{
    color_->Commit();
    selectedStyle_ = &style;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::OnColorChange
// Created: AGE 2008-01-16
// -----------------------------------------------------------------------------
void DrawerPanel::OnColorChange( const QColor& color )
{
    if( selectedDrawing_ )
        selectedDrawing_.ConstCast()->ChangeColor( color );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::StartDrawing
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void DrawerPanel::StartDrawing()
{
    if( selectedStyle_ )
    {
        Drawing_ABC* shape = model_.Create( *selectedStyle_, color_->GetColor() );
        if( selectedStyle_->GetType() == "line" )
            layer_.StartLine( *shape );
        else if( selectedStyle_->GetType() == "polygon" )
            layer_.StartPolygon( *shape );
        else if( selectedStyle_->GetType() == "point" )
            layer_.StartPoint( *shape );
        else
            throw std::runtime_error( __FUNCTION__ " Unhandled shape geometry." );
    }
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
        model_.Load( filename.ascii() );
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
        model_.Save( filename.ascii() );
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
    model_.Purge();
}
