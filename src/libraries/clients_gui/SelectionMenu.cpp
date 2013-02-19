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
#include "StandardIconProxyStyle.h"

#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"

#include "clients_kernel/Positions.h"
#include "clients_kernel/LocationVisitor_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SelectionMenu constructor
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
SelectionMenu::SelectionMenu( EntitySymbols& entitySymbols, ColorStrategy& colorStrategy, DrawingTypes& drawingTypes, kernel::GlTools_ABC& tools )
    : entitySymbols_( entitySymbols )
    , colorStrategy_( colorStrategy )
    , drawingTypes_( drawingTypes )
    , tools_( tools )
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
    void ApplyMousePress( Layer_ABC& layer, const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, QMouseEvent* mouse, Qt::MouseButton button )
    {
        if( button == Qt::LeftButton )
            layer.Select( selectable, ( mouse->modifiers() & Qt::ControlModifier ) != 0, ( mouse->modifiers() & Qt::ShiftModifier ) != 0 );
        else if( button == Qt::RightButton && !layer.IsReadOnly() )
            layer.ContextMenu( selectable, point, mouse->globalPos() );
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
                else if( const TacticalHierarchies* hierarchies = entity->Retrieve< TacticalHierarchies >() )   // Entity & Knowledge ?
                {
                    if( const kernel::Symbol_ABC* symbol = entity->Retrieve< kernel::TacticalHierarchies >() )
                    {
                        const std::string symbolName = symbol->GetSymbol();
                        const std::string levelName  = symbol->GetLevel();
                        pixmap = entitySymbols_.GetSymbol( *entity, symbolName, levelName, QSize( 64, 64 ), EntitySymbols::eColorWithModifier );
                    }
                    if( allIconsGenerated && pixmap.isNull() )
                        allIconsGenerated = false;
                }
            }
        }
    }
    return allIconsGenerated;
}

namespace
{
    class RichMenu : public QMenu
    {
    public:
                 RichMenu() : QMenu(), button_( Qt::NoButton ) {}
        virtual ~RichMenu() {}

        Qt::MouseButton GetButton() const { return button_; }

        virtual void mousePressEvent( QMouseEvent* event )
        {
            const Qt::MouseButton newButton = event->button();
            button_ = ( newButton == Qt::LeftButton || newButton == Qt::RightButton ) ? newButton : Qt::NoButton;
            QMenu::mousePressEvent( event );
        }

    private:
        Qt::MouseButton button_;
    };
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

    RichMenu menu;
    menu.setStyle( new StandardIconProxyStyle() );
    QAction* dummyEntry = menu.addAction( "" ); // Can't have a separator without an item before
    dummyEntry->setEnabled( false );
    dummyEntry->setFont( QFont( "Arial", 1 ) );

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
            QAction* action = menu.addAction( graphicalEntity->GetTooltip() );
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

    if( QAction* resultingAction = menu.exec( mouseEvent_->globalPos() ) )
    {
        const QString actionText = resultingAction->text();
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
                if( graphicalEntity->GetTooltip() == actionText )
                {
                    ApplyMousePress( *layer, *graphicalEntity, point_, &*mouseEvent_, menu.GetButton() );
                    icons_.clear();
                    return;
                }
            }
        }
    }
    icons_.clear();
}

// -----------------------------------------------------------------------------
// Name: SelectionMenu::ExecMenu
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void SelectionMenu::ExecMenu( const Layer_ABC::T_LayerElements& extractedElements, const geometry::Point2f& point,
                              const QPoint &globalPos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers )
{
    mouseEvent_.reset( new QMouseEvent( QEvent::None, globalPos, globalPos, button, Qt::NoButton, modifiers ) );

    if( extractedElements.size() == 1 && extractedElements.begin()->second.size() == 1 &&
        extractedElements.begin()->first && *extractedElements.begin()->second.begin() )    // Only one element extracted, classic way
    {
        ApplyMousePress( *extractedElements.begin()->first, **extractedElements.begin()->second.begin(), point, &*mouseEvent_, mouseEvent_->button() );
        return;
    }

    extractedElements_ = extractedElements;
    point_ = point;
    GenerateMenu();                                                                         // Several elements extracted, menu way
}
