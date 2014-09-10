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
#include "GlWidget.h"
#include "Gl3dWidget.h"
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
#include "clients_kernel/Options.h"
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
SelectionMenu::SelectionMenu( Controllers& controllers, EntitySymbols& entitySymbols, ColorStrategy& colorStrategy,
                              DrawingTypes& drawingTypes, GlTools_ABC& tools )
    : controllers_( controllers )
    , entitySymbols_( entitySymbols )
    , colorStrategy_( colorStrategy )
    , drawingTypes_( drawingTypes )
    , tools_( tools )
    , parent2d_( 0 )
    , parent3d_( 0 )
    , moreElements_( 0u )
    , mode3d_( false )
    , current_( 0 )
{
    controllers_.options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu destructor
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
SelectionMenu::~SelectionMenu()
{
    controllers_.options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::OnWidget2dChanged
// Created: ABR 2013-02-21
// -----------------------------------------------------------------------------
void SelectionMenu::OnWidget2dChanged( gui::GlWidget* parent )
{
    parent2d_ = parent;
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::OnWidget3dChanged
// Created: LGY 2013-03-13
// -----------------------------------------------------------------------------
void SelectionMenu::OnWidget3dChanged( gui::Gl3dWidget* parent )
{
    parent3d_ = parent;
}

namespace
{
    void ApplyMousePress( Layer_ABC& layer, const GraphicalEntity_ABC& selectable, QMouseEvent* mouse, Qt::MouseButton button )
    {
        if( button == Qt::LeftButton )
            layer.Select( selectable, ( mouse->modifiers() & Qt::ControlModifier ) != 0, ( mouse->modifiers() & Qt::ShiftModifier ) != 0 );
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
        drawingTemplate->GenerateSamplePixmap( tools_, r, g, b, markerPixelRatio );
        return drawingTemplate->GetSamplePixmap();
    }

    return QPixmap();
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::GenerateIcons
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
bool SelectionMenu::GenerateIcons()
{
    bool allIconsGenerated = true;
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

            QPixmap& pixmap = icons_[ graphicalEntity ];
            if( !pixmap.isNull() )
                continue;

            // Use something like that instead of the following crap when GraphicalEntity will be into clients_gui
            // pixmap = graphicalEntity->GetPixmap( entitySymbols_, colorStrategy_, drawingTypes_, tools_ );

            if( const Entity_ABC* entity = dynamic_cast< const Entity_ABC* >( graphicalEntity ) )
            {
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
                else if( const TacticalHierarchies* hierarchies = entity->Retrieve< TacticalHierarchies >() )   // Entity & Knowledge ?
                {
                    if( const kernel::Symbol_ABC* symbol = entity->Retrieve< kernel::TacticalHierarchies >() )
                    {
                        const std::string symbolName = symbol->GetSymbol();
                        const std::string levelName  = symbol->GetLevel();
                        pixmap = entitySymbols_.GetSymbol( *entity, symbolName, levelName, QSize( 64, 64 ), EntitySymbols::eColorWithModifier );
                    }
                    if( allIconsGenerated && ( !mode3d_ && pixmap.isNull() ) )
                        allIconsGenerated = false;
                }
            }
        }
    }
    return allIconsGenerated;
}

namespace
{
    class Menu_ABC : public kernel::ContextMenu
    {
    public:
                 Menu_ABC( QWidget* parent ) : kernel::ContextMenu( parent ){}
        virtual ~Menu_ABC() {}

        virtual Qt::MouseButton GetButton() const = 0;
    };

    template< typename T >
    class RichMenu : public Menu_ABC
    {
    public:
        explicit RichMenu( T* parent ) : Menu_ABC( parent ), button_( Qt::NoButton ), parent_( parent ) {}
        virtual ~RichMenu() {}

        virtual Qt::MouseButton GetButton() const { return button_; }

        virtual void mousePressEvent( QMouseEvent* event )
        {
            const Qt::MouseButton newButton = event->button();
            button_ = ( newButton == Qt::LeftButton || newButton == Qt::RightButton ) ? newButton : Qt::NoButton;
            QMenu::mousePressEvent( event );
        }
        virtual void wheelEvent( QWheelEvent* event )
        {
            parent_->wheelEvent( event );
        }

    private:
        T* parent_;
        Qt::MouseButton button_;
    };

    QAction* GenerateAction( const kernel::GraphicalEntity_ABC& graphicalEntity, Menu_ABC& menu, QMouseEvent& event, gui::Layer_ABC& /*layer*/ )
    {
        if( event.button() == Qt::RightButton )
        {
            kernel::ContextMenu* context = new kernel::ContextMenu( &menu );
            context->setTitle( graphicalEntity.GetTooltip() );
            return menu.addMenu( context );
        }
        else
            return menu.addAction( graphicalEntity.GetTooltip() );
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::GenerateMenu
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void SelectionMenu::GenerateMenu()
{
    if( !GenerateIcons() )
    {
        QTimer::singleShot( 100, this, SLOT( GenerateMenu() ) );
        return;
    }

    std::unique_ptr< Menu_ABC > menu;
    if( mode3d_ )
        menu.reset( new RichMenu< gui::Gl3dWidget >( parent3d_ ) );
    else
        menu.reset( new RichMenu< gui::GlWidget >( parent2d_ ) );
    connect( menu.get(), SIGNAL( hovered( QAction* ) ), this, SLOT( OnSelectionChanged( QAction* ) ) );

    menu->setStyle( new StandardIconProxyStyle() );

    // merge with default menu
    if( mouseEvent_->button() == Qt::RightButton )
        controllers_.actions_.ContextMenu( point_, kernel::Nothing(), *menu );
    menu->FillMenu();

    for( auto extractedPair = extractedElements_.begin(); extractedPair != extractedElements_.end(); ++extractedPair )
    {
        Layer_ABC* layer = extractedPair->first;
        kernel::GraphicalEntity_ABC::T_GraphicalEntities& entities = extractedPair->second;
        if( !layer )
            continue;
        menu->addSeparator()->setText( layer->GetName() );
        for( auto extractedElement = entities.begin(); extractedElement != entities.end(); ++extractedElement )
        {
            const GraphicalEntity_ABC* graphicalEntity = *extractedElement;
            if( !graphicalEntity )
                continue;

            QAction* action = GenerateAction( *graphicalEntity, *menu, *mouseEvent_, *layer );
            QPixmap& pixmap = icons_[ graphicalEntity ];
            if( !action )
                continue;

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

    if( moreElements_!= 0u )
    {
        QAction* action = menu->addAction( tr( "And %1 more elements..." ).arg( QString::number( moreElements_ ) ) );
        action->setEnabled( false );
        action->setFont( QFont( "Arial", -1, -1, true ) );
    }

    if( QAction* resultingAction = menu->QMenu::exec( mouseEvent_->globalPos() ) )
    {
        T_Result result = GetSelected( resultingAction->text() );
        if( result.first && result.second )
            ApplyMousePress( *result.first, *result.second, &*mouseEvent_, menu->GetButton() );
    }
    icons_.clear();
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
        ApplyMousePress( *extractedElements.begin()->first, **extractedElements.begin()->second.begin(), &*mouseEvent_, mouseEvent_->button() );
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
// Name: SelectionMenu::OptionChanged
// Created: LGY 2013-03-13
// -----------------------------------------------------------------------------
void SelectionMenu::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "3D" )
        mode3d_ = value.To< bool >();
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::OnSelectionChanged
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
void SelectionMenu::OnSelectionChanged( QAction* action )
{
    if( mouseEvent_->button() == Qt::RightButton )
    {
        if( !action || ( current_ && current_ == action ) )
            return;

        T_Result result = GetSelected( action->text() );
        if( !result.first || !result.second )
            return;

        kernel::ContextMenu* context = new kernel::ContextMenu( action->parentWidget() );
        result.first->FillContextMenu( static_cast< const kernel::Entity_ABC& >( *result.second ), *context );
        context->FillMenu();
        action->setMenu( context );

        current_ = action;
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::GetSelected
// Created: LGY 2013-04-08
// -----------------------------------------------------------------------------
std::pair< Layer_ABC*, const GraphicalEntity_ABC* > SelectionMenu::GetSelected( const QString& text )
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
            if( graphicalEntity->GetTooltip() == text )
                return std::make_pair( layer, graphicalEntity );
        }
    }
    return std::make_pair( nullptr, nullptr );
}
