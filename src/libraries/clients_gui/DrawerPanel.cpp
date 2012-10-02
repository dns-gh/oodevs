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
#include "Drawing.h"
#include "DrawingCategory.h"
#include "DrawingTemplate.h"
#include "DrawingCategoryItem.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "resources.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerPanel constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::DrawerPanel( QWidget* parent, PanelStack_ABC& panel, ParametersLayer& layer, kernel::Controllers& controllers, DrawerModel& model, const tools::ExerciseConfig& config )
    : InfoPanel_ABC( parent, panel, tr( "Drawings" ) )
    , controllers_    ( controllers )
    , layer_          ( layer )
    , model_          ( model )
    , config_         ( config )
    , selectedStyle_  ( 0 )
    , selectedDrawing_( controllers )
    , selectedEntity_ ( controllers )
{
    Q3VBox* vbox = new Q3VBox( this );
    {
        Q3HBox* box = new Q3HBox( vbox );
        box->layout()->setAlignment( Qt::AlignCenter );
        box->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
        box->setBackgroundMode( Qt::PaletteButton );
        box->setFrameStyle( QFrame::StyledPanel | Q3Frame::Raised );

        QToolButton* btn = new QToolButton( box );
        btn->setAutoRaise( true );
        btn->setIconSet( MAKE_PIXMAP( open ) );
        QToolTip::add( btn, tr( "Import drawings file" ) );
        connect( btn, SIGNAL( clicked() ), SLOT( Open() ) );

        btn = new QToolButton( box );
        btn->setAutoRaise( true );
        btn->setIconSet( MAKE_PIXMAP( save ) );
        QToolTip::add( btn, tr( "Export drawings file" ) );
        connect( btn, SIGNAL( clicked() ), SLOT( Save() ) );

        btn = new QToolButton( box );
        btn->setAutoRaise( true );
        btn->setIconSet( MAKE_PIXMAP( cross ) );
        btn->setFixedSize( 25, 25 );
        QToolTip::add( btn, tr( "Clear drawings" ) );
        connect( btn, SIGNAL( clicked() ), SLOT( Clear() ) );

        color_ = new ColorButton( box, "" );
        toolBox_ = new QToolBox( vbox );
        toolBox_->setMargin( 0 );
        toolBox_->setBackgroundColor( Qt::white );
        connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChange( const QColor& ) ) );

        btn = new QToolButton( box );
        btn->setAutoRaise( true );
        btn->setIconSet( MAKE_PIXMAP( pencil ) );
        btn->setFixedSize( 25, 25 );
        QToolTip::add( btn, tr( "Start drawing" ) );
        connect( btn, SIGNAL( clicked() ), SLOT( StartDrawing() ) );
    }
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, vbox );
    {
        new QLabel( tr( "Parent:" ), group );
        parentLabel_ = new QLabel( "---", group );
    }
    toolBox_ = new QToolBox( vbox );
    toolBox_->setMargin( 0 );
    toolBox_->setBackgroundColor( Qt::white );
    setWidget( vbox );

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
// Name: DrawerPanel::setEnabled
// Created: JSR 2011-06-14
// -----------------------------------------------------------------------------
void DrawerPanel::setEnabled( bool b )
{
    InfoPanel_ABC::setEnabled( b );
    for( T_CategoryItems::iterator it = categories_.begin(); it != categories_.end(); ++it )
        it->second->setEnabled( b );
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
    T_CategoryItems::iterator it = categories_.find( &category );
    if( it != categories_.end() )
    {
        toolBox_->removeItem( it->second );
        delete it->second;
        categories_.erase( it );
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
// Name: DrawerPanel::BeforeSelection
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
void DrawerPanel::BeforeSelection()
{
    selectedDrawing_ = 0;
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::AfterSelection
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
void DrawerPanel::AfterSelection()
{
    if( selectedEntity_ )
        parentLabel_->setText( selectedEntity_->GetName() );
    else if( selectedDrawing_ && selectedDrawing_->GetDiffusionEntity() )
        parentLabel_->setText( selectedDrawing_->GetDiffusionEntity()->GetName() );
    else
        parentLabel_->setText( "---" );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Select
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
void DrawerPanel::Select( const kernel::Drawing_ABC& element )
{
    selectedDrawing_ = static_cast< const Drawing* >( &element );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Select
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
void DrawerPanel::Select( const kernel::Automat_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Select
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
void DrawerPanel::Select( const kernel::Formation_ABC& element )
{
    selectedEntity_ = &element;
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
    if( !selectedEntity_ && ( !selectedDrawing_ || !selectedDrawing_->GetDiffusionEntity() ) )
        return;
    if( selectedStyle_ )
    {
        Drawing* shape = static_cast< Drawing* >( model_.Create( *selectedStyle_, color_->GetColor(), selectedEntity_ ) );
        if( selectedStyle_->GetType() == "line" )
            layer_.StartLine( *shape );
        else if( selectedStyle_->GetType() == "polygon" )
            layer_.StartPolygon( *shape );
        else if( selectedStyle_->GetType() == "point" )
            layer_.StartPoint( *shape );
        else if( selectedStyle_->GetType() == "circle" )
            layer_.StartCircle( *shape );
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
    QString filename = Q3FileDialog::getOpenFileName( config_.BuildExerciseChildFile( "" ).c_str(), tr( "Drawings (*.xml)" ), this, 0, tr( "Load drawings file" ) );
    if( filename.isEmpty() )
        return;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    try
    {
        model_.Load( config_.GetLoader(), filename.toAscii().constData() );
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
    QString filename = Q3FileDialog::getSaveFileName( config_.BuildExerciseChildFile( "" ).c_str(), tr( "Drawings (*.xml)" ), this, 0, tr( "Save drawings to file" ) );
    if( filename.isEmpty() )
        return;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    if( !filename.endsWith( ".xml" ) )
        filename.append( ".xml" );
    try
    {
        tools::SchemaWriter schemaWriter; //$$ Probablement � remonter
        model_.Serialize( filename.toAscii().constData(), schemaWriter );
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
