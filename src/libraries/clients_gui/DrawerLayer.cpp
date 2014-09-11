// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerLayer.h"
#include "moc_DrawerLayer.cpp"
#include "Drawing.h"
#include "ModelObserver_ABC.h"
#include "Tools.h"
#include "Viewport2d.h"
#include "clients_kernel/Controller.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerLayer constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::DrawerLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile, ModelObserver_ABC& model )
    : EntityLayer< kernel::Drawing_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Drawers )
    , parameters_( parameters )
    , tools_     ( tools )
    , model_     ( model )
    , selected_  ( 0 )
{
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::~DrawerLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::NotifyContextMenu
// Created: LGY 2014-05-12
// -----------------------------------------------------------------------------
void DrawerLayer::ContextMenu( const kernel::GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const kernel::Entity_ABC& entity = static_cast< const kernel::Entity_ABC& >( selectable );
    const kernel::Drawing_ABC& drawing = static_cast< const kernel::Drawing_ABC& >( entity );
    controllers_.actions_.ContextMenu( entity, drawing, point, where );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::FillContextMenu
// Created: LGY 2014-05-12
// -----------------------------------------------------------------------------
void DrawerLayer::FillContextMenu( const kernel::GraphicalEntity_ABC& selectable, kernel::ContextMenu& menu )
{
    const kernel::Entity_ABC& entity = static_cast< const kernel::Entity_ABC& >( selectable );
    const kernel::Drawing_ABC& drawing = static_cast< const kernel::Drawing_ABC& >( entity );
    controllers_.actions_.ContextMenu( entity, drawing, menu );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::NotifyContextMenu
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawerLayer::NotifyContextMenu( const kernel::Drawing_ABC& drawing, kernel::ContextMenu& menu )
{
    if( selected_ != &drawing )
    {
        std::vector< const kernel::Drawing_ABC* > vector;
        vector.push_back( &drawing );
        NotifySelectionChanged( vector );
    }
    menu.InsertItem( "Creation", tools::translate( "gui::DrawerLayer", "Edit..." ), this, SLOT( OnEditDrawing() ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::OnEditDrawing
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::OnEditDrawing()
{
    if( selected_ )
        static_cast< Drawing* >( const_cast< kernel::Drawing_ABC* >( selected_ ) )->Edit( parameters_ );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::NotifyDeleted
// Created: MMC 2013-04-18
// -----------------------------------------------------------------------------
void DrawerLayer::NotifyDeleted( const kernel::Drawing_ABC& drawing )
{
    if( selected_ == &drawing )
        selected_ = 0;
    EntityLayer< kernel::Drawing_ABC >::NotifyDeleted( drawing );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Paint
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::Paint( const geometry::Rectangle2f& viewport )
{
    viewport_ = viewport;
    Layer::Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::ShouldDisplay
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
bool DrawerLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    const kernel::Entity_ABC* diffusion = static_cast< const kernel::Drawing_ABC& >( entity ).GetDiffusionEntity();
    if( diffusion )
        return EntityLayer< kernel::Drawing_ABC >::ShouldDisplay( *diffusion );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void DrawerLayer::NotifySelectionChanged( const std::vector< const kernel::Drawing_ABC* >& elements )
{
    EntityLayer< kernel::Drawing_ABC >::NotifySelectionChanged( elements );
    selected_ =  elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Draw
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::Draw( const kernel::Entity_ABC& entity, Viewport_ABC&, bool pickingMode )
{
    assert( dynamic_cast< const Drawing* >( &entity ) );
    if( ShouldDisplay( entity ) )
    {
        if( pickingMode )
            tools_.RenderPicking( std::make_pair( entity.GetId(), type_ ) );
        SelectColor( entity );
        static_cast< const Drawing& >( entity ).Draw( viewport_, tools_, &entity == selected_ );
        if( pickingMode )
            tools_.Picking();
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleKeyPress
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleKeyPress( QKeyEvent* k )
{
    if( !selected_ )
        return false;
    const int key = k->key();
    if( key == Qt::Key_Backspace || key == Qt::Key_Delete )
    {
        if( selected_ )
            model_.DeleteEntity( *selected_ );
        return true;
    }
    return EntityLayer< kernel::Drawing_ABC >::HandleKeyPress( k );
}
