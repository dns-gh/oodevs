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
#include "ParametersLayer.h"
#include "ColorButton.h"
#include "DrawerModel.h"
#include "Drawing.h"
#include "DrawingCategory.h"
#include "DrawingTemplate.h"
#include "DrawingCategoryItem.h"
#include "FileDialog.h"
#include "RichGroupBox.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include "RichWidget.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "resources.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

namespace
{
    QPixmap CreatePixmap( QVector< qreal >& dashes )
    {
        QPixmap pix( 60, 10 );
        pix.fill( Qt::transparent );
        QPainter painter( &pix );
        QPen pen( QBrush( Qt::black ), 3 );
        if( !dashes.empty() )
            pen.setDashPattern( dashes );
        painter.setPen( pen );
        painter.drawLine( 1, 5, 59, 5 );
        return pix;
    }
}

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
    , dashStyle_      ( eSolid )
{
    SubObjectName subObject( "DrawerPanel" );

    //main drawing box box panel
    Q3HBox* box = new Q3HBox();
    box->layout()->setAlignment( Qt::AlignCenter );
    box->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    box->setBackgroundMode( Qt::PaletteButton );
    box->setFrameStyle( QFrame::StyledPanel | Q3Frame::Raised );
    box->setSpacing( 2 );

    RichWidget< QToolButton >* btn = new RichWidget< QToolButton >( "LoadDrawings", box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( open ) );
    QToolTip::add( btn, tr( "Load drawing file" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Open() ) );

    btn = new RichWidget< QToolButton >( "saveDrawings", box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( save ) );
    QToolTip::add( btn, tr( "Save drawings to file" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Save() ) );

    btn = new RichWidget< QToolButton >( "clearDrawings", box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( cross ) );
    btn->setFixedSize( 25, 25 );
    QToolTip::add( btn, tr( "Clear drawings" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Clear() ) );

    color_ = new ColorButton( "color", box, "" );
    toolBox_ = new QToolBox();
    toolBox_->setMargin( 0 );
    toolBox_->setBackgroundColor( Qt::white );
    connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChange( const QColor& ) ) );

    btn = new RichWidget< QToolButton >( "startDrawing", box );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_PIXMAP( pencil ) );
    btn->setFixedSize( 25, 25 );
    QToolTip::add( btn, tr( "Start drawing" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( StartDrawing() ) );

    RichWidget< QComboBox >* combo = new RichWidget< QComboBox >( "line", box );;
    combo->setEditable( false );

    QVector< qreal > dashes;
    QPixmap solid = CreatePixmap( dashes );
    combo->setIconSize( solid.size() );
    combo->addItem( QIcon( solid ), "" );

    dashes << 1 << 2;
    QPixmap dashed = CreatePixmap( dashes );
    combo->addItem( dashed, "" );

    dashes.clear();
    dashes << 5 << 4 << 1 << 4;
    QPixmap dashdot =  CreatePixmap( dashes );
    combo->addItem( dashdot, "" );
    connect( combo, SIGNAL( currentIndexChanged( int ) ), SLOT( OnLineChanged( int ) ) );

    //parent group
    QLabel* parentLabelText = new QLabel( tr( "Parent:" ) );
    parentLabel_ = new QLabel( "---" );

    RichGroupBox* group = new RichGroupBox( "parentgroup" );
    QHBoxLayout* groupLayout = new QHBoxLayout( group );
    groupLayout->addWidget( parentLabelText );
    groupLayout->addWidget( parentLabel_ );
    
    //toolbox
    toolBox_ = new QToolBox();
    toolBox_->setMargin( 0 );
    toolBox_->setBackgroundColor( Qt::white );

    QWidget* vbox = new QWidget( this );
    QVBoxLayout* vBoxLayout = new QVBoxLayout( vbox );
    vBoxLayout->addWidget( box );
    vBoxLayout->addWidget( group );
    vBoxLayout->addWidget( toolBox_ );
    vBoxLayout->addStretch( 1 );
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
    if( selectedStyle_ )
    {
        if( selectedStyle_->GetType() == "line" )
            layer_.StartLine( *this );
        else if( selectedStyle_->GetType() == "polygon" )
            layer_.StartPolygon( *this );
        else if( selectedStyle_->GetType() == "point" )
            layer_.StartPoint( *this );
        else if( selectedStyle_->GetType() == "circle" )
            layer_.StartCircle( *this );
        else if( selectedStyle_->GetType() == "curve" )
            layer_.StartCurve( *this );
        else
            throw MASA_EXCEPTION( "Unhandled shape geometry." );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Open
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerPanel::Open()
{
    tools::Path filename = FileDialog::getOpenFileName( this, tr( "Load drawing file" ), config_.BuildExerciseChildFile( "" ), tr( "Drawings (*.xml)" ) );
    if( filename.IsEmpty() )
        return;
    filename.MakePreferred();
    try
    {
        model_.Load( config_.GetLoader(), filename );
    }
    catch( const xml::exception& )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "'%1' is not a valid drawing file." ).arg( filename.ToUTF8().c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Save
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerPanel::Save()
{
    tools::Path filename = FileDialog::getSaveFileName( this, tr( "Save drawings to file" ), config_.BuildExerciseChildFile( "" ), tr( "Drawings (*.xml)" ) );
    if( filename.IsEmpty() )
        return;
    filename.MakePreferred();
    if( filename.Extension() != ".xml" )
        filename.ReplaceExtension( ".xml" );
    try
    {
        tools::SchemaWriter schemaWriter; //$$ Probablement à remonter
        model_.Serialize( filename, schemaWriter );
    }
    catch( const xml::exception& )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Unable to save drawings to file '%1'. \nPlease check access permissions or write protection." ).arg( filename.ToUTF8().c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::Clear
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerPanel::Clear()
{
    model_.Purge();
    selectedDrawing_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::OnLineChanged
// Created: LGY 2013-04-15
// -----------------------------------------------------------------------------
void DrawerPanel::OnLineChanged( int index )
{
    dashStyle_ = E_Dash_style( index );
}


// -----------------------------------------------------------------------------
// Name: DrawerPanel::Handle
// Created: LGY 2014-05-14
// -----------------------------------------------------------------------------
void DrawerPanel::Handle( kernel::Location_ABC& location )
{
    if( selectedStyle_ )
        model_.Create( *selectedStyle_, color_->GetColor(), selectedEntity_, dashStyle_, location );
}
