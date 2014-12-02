// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SelectionMenu.h"
#include "moc_SelectionMenu.cpp"

#include "ColorStrategy.h"
#include "DrawingCategory.h"
#include "DrawerShape.h"
#include "DrawingTemplate.h"
#include "DrawingTypes.h"
#include "EntitySymbols.h"
#include "ImageWrapper.h"
#include "GLOptions.h"
#include "GLMainProxy.h"
#include "GLStackedWidget.h"
#include "GL2DWidget.h"
#include "StandardIconProxyStyle.h"

#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "tools/GeneralConfig.h"

using namespace gui;
using namespace kernel;

namespace
{
    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gui" ) / name + ".png" ) );
    }
    static const unsigned int MAX_ELEMENT = 16u;
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu constructor
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
SelectionMenu::SelectionMenu( Controllers& controllers,
                              EntitySymbols& entitySymbols,
                              ColorStrategy& colorStrategy,
                              DrawingTypes& drawingTypes,
                              GLMainProxy& mainProxy )
    : controllers_( controllers )
    , entitySymbols_( entitySymbols )
    , colorStrategy_( colorStrategy )
    , drawingTypes_( drawingTypes )
    , mainProxy_( mainProxy )
    , moreElements_( 0u )
    , current_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu destructor
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
SelectionMenu::~SelectionMenu()
{
    // NOTHING
}

namespace
{
    void ApplyMousePress( Layer_ABC& layer, unsigned int id, QMouseEvent* mouse, Qt::MouseButton button )
    {
        if( button == Qt::LeftButton )
            layer.Select( id, ( mouse->modifiers() & Qt::ControlModifier ) != 0 );
    }

    void FillPixmapBackground( QPixmap& pix )
    {
        QColor backGroundColor;
        backGroundColor.setRgbF( 0.9f, 0.9f, 0.9f );
        pix.fill( backGroundColor );
    }

    void DrawPixmapFrame( QPainter& painter )
    {
        painter.setPen( QPen( QBrush( Qt::black ), 1 ) );
        painter.drawLine( 0, 0, 0, 63 );
        painter.drawLine( 0, 0, 63, 0 );
        painter.drawLine( 63, 63, 0, 63 );
        painter.drawLine( 63, 63, 63, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::ExtractDrawingSample
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
QPixmap SelectionMenu::ExtractDrawingSample( const std::string& code, const QColor& color, const std::string& category /*= ""*/, float markerPixelRatio /*= 1.f*/ ) const
{
    return ExtractDrawingSample( code, static_cast< float >( color.redF() ), static_cast< float >( color.greenF() ), static_cast< float >( color.blueF() ), category, markerPixelRatio );
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::ExtractDrawingSample
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
QPixmap SelectionMenu::ExtractDrawingSample( const std::string& code, float r, float g, float b, const std::string& category /* = "" */, float markerPixelRatio /*= 1.f*/ ) const
{
    const DrawingCategory* drawingCategory = 0;
    DrawingTemplate* drawingTemplate = 0;

    if( !category.empty() )
        drawingCategory = drawingTypes_.Find( category.c_str() );

    if( drawingCategory == 0 )
    {
        tools::Iterator< const DrawingCategory& > it( drawingTypes_.CreateIterator() );
        while( !drawingTemplate && it.HasMoreElements() )
        {
            drawingCategory = &it.NextElement();
            drawingTemplate = drawingCategory->Find( code );
        }
    }
    else
        drawingTemplate = drawingCategory->Find( code );

    if( drawingTemplate )
    {
        drawingTemplate->GenerateSamplePixmap( r, g, b, markerPixelRatio );
        return drawingTemplate->GetSamplePixmap();
    }

    return QPixmap();
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::GenerateIcons
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void SelectionMenu::GenerateIcons()
{
    for( auto extractedPair = extractedElements_.begin(); extractedPair != extractedElements_.end(); ++extractedPair )
    {
        Layer_ABC* layer = extractedPair->first;
        kernel::GraphicalEntity_ABC::T_GraphicalEntities& entities = extractedPair->second;
        if( !layer )
            continue;
        for( auto extractedElement = entities.begin(); extractedElement != entities.end(); ++extractedElement )
        {
            const GraphicalEntity_ABC* graphicalEntity = *extractedElement;
            if( !graphicalEntity )
                continue;

            // Use something like that instead of the following crap when GraphicalEntity will be into clients_gui
            // pixmap = graphicalEntity->GetPixmap( entitySymbols_, colorStrategy_, drawingTypes_, proxy_ );
            if( const Entity_ABC* entity = dynamic_cast< const Entity_ABC* >( graphicalEntity ) )
            {
                QPixmap& pixmap = icons_[ entity->GetId() ];
                if( !pixmap.isNull() )
                    continue;

                if( entity->GetTypeName() == kernel::TacticalLine_ABC::typeName_ )                              // Tactical line
                {
                    const TacticalHierarchies& tacticalHierarchy = entity->Get< TacticalHierarchies >();
                    const std::string symbol = tacticalHierarchy.GetSymbol();
                    const kernel::Entity_ABC* superior = tacticalHierarchy.GetSuperior();
                    if( !superior )
                        superior = &tacticalHierarchy.GetTop();

                    QColor color = colorStrategy_.FindColorWithModifiers( *superior );
                    if( !symbol.empty() )
                        pixmap = ExtractDrawingSample( symbol, color, "", 5.f );

                    if( symbol.empty() || pixmap.isNull() )
                    {
                        QPixmap pix( 64, 64 );
                        FillPixmapBackground( pix );
                        QPainter painter( &pix );
                        painter.setPen( QPen( QBrush( color ), 2 ) );
                        painter.drawLine( SYMBOL_ICON_MARGIN / 2, 32, 64 - SYMBOL_ICON_MARGIN / 2, 32 );
                        DrawPixmapFrame( painter );
                        pixmap = pix;
                    }
                }
                else if( entity->GetTypeName() == kernel::Object_ABC::typeName_ )                               // Object
                {
                    const Object_ABC& object = static_cast< const Object_ABC& >( *entity );
                    const std::string symbol = object.GetSymbol();

                    pixmap = ExtractDrawingSample( symbol, colorStrategy_.FindColorWithModifiers( object ) );
                    // $$$$ ABR 2013-02-04: TODO: Use object location type to display base shape if no drawing template associated
                }
                else if( entity->GetTypeName() == kernel::Drawing_ABC::typeName_ )                              // Drawing
                {
                    const DrawerShape& drawerShape = static_cast< const DrawerShape& >( *entity );
                    pixmap = ExtractDrawingSample( drawerShape.GetDrawingTemplate().GetCode().toStdString(),
                                                   drawerShape.GetColor(),
                                                   drawerShape.GetDrawingTemplate().GetCategory().toStdString(), 5.f );
                }
                else if( entity->GetTypeName() == kernel::Population_ABC::typeName_ )                           // Population
                {
                    QColor color = colorStrategy_.FindColorWithModifiers( *entity );
                    QPixmap pix( 64, 64 );
                    FillPixmapBackground( pix );
                    QPainter painter( &pix );
                    painter.setBrush( QBrush( color ) );
                    painter.drawEllipse( 4, 4, 55, 55 );
                    DrawPixmapFrame( painter );
                    pixmap = pix;
                }
                else if( entity->GetTypeName() == kernel::UrbanObject_ABC::typeName_ )                          // Urban block
                {
                    UrbanBlockColor urbanColor = entity->Get< UrbanColor_ABC >().GetColor();
                    QColor color( urbanColor.red_, urbanColor.green_, urbanColor.blue_, 255 );
                    pixmap = QPixmap( 32, 32 );
                    pixmap.fill( color );
                }
                else if( entity->GetTypeName() == kernel::Pathfind_ABC::typeName_ )
                {
                    static const QPixmap pathfind( "resources/symbols/pathfind.png" );
                    pixmap = pathfind;
                }
                else if( const auto* hierarchies = entity->Retrieve< TacticalHierarchies >() )   // Entity & Knowledge ?
                {
                    if( const auto* symbol = entity->Retrieve< kernel::TacticalHierarchies >() )
                    {
                        const std::string symbolName = symbol->GetSymbol();
                        const std::string levelName = symbol->GetLevel();
                        pixmap = entitySymbols_.GetSymbol( *entity, symbolName, levelName, EntitySymbols::eColorWithModifier );
                    }
                }
            }
        }
    }
}

namespace
{
    class RichMenu : public kernel::ContextMenu
    {
    public:
        explicit RichMenu( const GLView_ABC::T_View& view )
            : kernel::ContextMenu( 0 )
            , button_( Qt::NoButton )
            , view_( view )
        {
        }
        virtual ~RichMenu() {}

        Qt::MouseButton GetButton() const { return button_; }

        virtual void mousePressEvent( QMouseEvent* event )
        {
            const Qt::MouseButton newButton = event->button();
            button_ = ( newButton == Qt::LeftButton || newButton == Qt::RightButton ) ? newButton : Qt::NoButton;
            QMenu::mousePressEvent( event );
        }
        virtual void wheelEvent( QWheelEvent* event )
        {
            if( view_ )
                view_->WheelEvent( event );
        }

    private:
        GLView_ABC::T_View view_;
        Qt::MouseButton button_;
    };

    QAction* GenerateAction( const QString& tooltip, QMenu& menu, QMouseEvent& event )
    {
        if( event.button() == Qt::RightButton )
        {
            kernel::ContextMenu* context = new kernel::ContextMenu( &menu );
            context->setTitle( tooltip );
            return menu.addMenu( context );
        }
        else
            return menu.addAction( tooltip );
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::GenerateMenu
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void SelectionMenu::GenerateMenu()
{
    GenerateIcons();
    RichMenu menu( mainProxy_.GetHoveredView() );
    connect( &menu, SIGNAL( hovered( QAction* ) ), this, SLOT( OnSelectionChanged( QAction* ) ) );

    menu.setStyle( new StandardIconProxyStyle() );

    // merge with default menu
    if( mouseEvent_->button() == Qt::RightButton )
        controllers_.actions_.ContextMenu( this, point_, kernel::Nothing(), menu );
    menu.FillMenu();

    for( auto extractedPair = extractedElements_.begin(); extractedPair != extractedElements_.end(); ++extractedPair )
    {
        Layer_ABC* layer = extractedPair->first;
        kernel::GraphicalEntity_ABC::T_GraphicalEntities& entities = extractedPair->second;
        if( !layer )
            continue;
        menu.addSeparator()->setText( layer->GetName() );
        for( auto extractedElement = entities.begin(); extractedElement != entities.end(); ++extractedElement )
        {
            const GraphicalEntity_ABC* graphicalEntity = *extractedElement;
            if( !graphicalEntity )
                continue;

            if( const Entity_ABC* entity = dynamic_cast< const Entity_ABC* >( graphicalEntity ) )
            {
                QAction* action = GenerateAction( entity->GetTooltip(), menu, *mouseEvent_ );
                if( !action )
                    continue;
                action->setData( QVariant::fromValue( entity->GetId() ) );

                entityLayer_[ entity->GetId() ] = layer;
                const QPixmap& pixmap = icons_[ entity->GetId() ];
                if( !pixmap.isNull() )
                    action->setIcon( pixmap );
                else
                {
                    QPixmap pix( 64, 64 );
                    FillPixmapBackground( pix );
                    QPainter painter( &pix );
                    DrawPixmapFrame( painter );
                    action->setIcon( pix );
                }
            }
        }
    }

    if( moreElements_!= 0u )
    {
        QAction* action = menu.addAction( tr( "And %1 more elements..." ).arg( QString::number( moreElements_ ) ) );
        action->setEnabled( false );
        action->setFont( QFont( "Arial", -1, -1, true ) );
    }

    if( QAction* resultingAction = menu.QMenu::exec( mouseEvent_->globalPos() ) )
    {
        const auto id = resultingAction->data().toUInt();
        auto it = entityLayer_.find( id );
        if( it != entityLayer_.end() )
            ApplyMousePress( *it->second, id, &*mouseEvent_, menu.GetButton() );
    }
    icons_.clear();
    entityLayer_.clear();
    moreElements_ = 0;
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::ExecMenu
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void SelectionMenu::ExecMenu( const Layer_ABC::T_LayerElements& extractedElements, const geometry::Point2f& point,
                              const QPoint& globalPos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers )
{
    mouseEvent_.reset( new QMouseEvent( QEvent::None, globalPos, globalPos, button, Qt::NoButton, modifiers ) );

    if( extractedElements.size() == 1 && extractedElements.begin()->second.size() == 1 &&
        extractedElements.begin()->first && *extractedElements.begin()->second.begin() )    // Only one element extracted, classic way
    {
        ApplyMousePress( *extractedElements.begin()->first, dynamic_cast< const Entity_ABC* >( *extractedElements.begin()->second.begin() )->GetId(), &*mouseEvent_, mouseEvent_->button() );
        return;
    }

    FilterElement( extractedElements );
    point_ = point;
    GenerateMenu();                                                                         // Several elements extracted, menu way
}

namespace
{
    std::size_t GetEntityCount( const Layer_ABC::T_LayerElements& extractedElements )
    {
        std::size_t count = 0u;
        for( auto extractedPair = extractedElements.begin(); extractedPair != extractedElements.end(); ++extractedPair )
            count += extractedPair->second.size();
        return count;
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::FilterElement
// Created: LGY 2013-02-28
// -----------------------------------------------------------------------------
void SelectionMenu::FilterElement( const Layer_ABC::T_LayerElements& extractedElements )
{
    std::size_t total = GetEntityCount( extractedElements );
    if( total <= MAX_ELEMENT )
        extractedElements_ = extractedElements;
    else
    {
        unsigned int count = 0u;
        std::size_t index = 0u;
        Layer_ABC::T_LayerElements filteredElements;
        while( count < MAX_ELEMENT )
        {
            for( auto extractedPair = extractedElements.begin(); extractedPair != extractedElements.end(); ++extractedPair )
            {
                Layer_ABC* layer = extractedPair->first;
                const kernel::GraphicalEntity_ABC::T_GraphicalEntities& entities = extractedPair->second;
                if( !layer )
                    continue;
                if( entities.size() > index )
                {
                    filteredElements[ layer ].push_back( entities.at( index ) );
                    ++count;
                }
            }
            ++index;
        }
        extractedElements_ = filteredElements;
        moreElements_ = static_cast< unsigned int > ( total ) - MAX_ELEMENT;
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::OnSelectionChanged
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
void SelectionMenu::OnSelectionChanged( QAction* action )
{
    if( mouseEvent_->button() == Qt::RightButton )
    {
        const auto id = action->data().toUInt();
        if( !action || current_ == action )
            return;
        auto it = entityLayer_.find( id );
        if( it == entityLayer_.end() )
            return;

        kernel::ContextMenu* context = new kernel::ContextMenu( action->parentWidget() );
        it->second->FillContextMenu( id, *context );
        context->FillMenu();
        action->setMenu( context );
        current_ = action;
    }
}
