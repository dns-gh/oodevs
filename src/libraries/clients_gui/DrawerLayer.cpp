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
#include "Tools.h"
#include "Viewport2d.h"
#include "clients_kernel/Controller.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerLayer constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::DrawerLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Drawing_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Drawers )
    , parameters_( parameters )
    , tools_     ( tools )
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
    menu.InsertItem( "Creation", tools::translate( "gui::DrawerLayer", "Edit drawing..." ), this, SLOT( OnEditDrawing() ) );
    menu.InsertItem( "Creation", tools::translate( "gui::DrawerLayer", "Erase drawing" )  , this, SLOT( OnDeleteDrawing() ) );
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
// Name: DrawerLayer::OnDeleteDrawing
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void DrawerLayer::OnDeleteDrawing()
{
    if( selected_ )
    {
        const kernel::Drawing_ABC* selected = selected_;
        selected_ = 0;
        controllers_.controller_.Delete( *const_cast< kernel::Drawing_ABC* >( selected ) );
        controllers_.actions_.DeselectAll();
        delete selected;
    }
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
            tools_.RegisterObjectPicking( std::make_pair( entity.GetId(), type_ ) );
        static_cast< const Drawing& >( entity ).Draw( pickingMode ? tools_.GlobalViewport() : viewport_, tools_, &entity == selected_ );
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
        OnDeleteDrawing();
        return true;
    }
    return EntityLayer< kernel::Drawing_ABC >::HandleKeyPress( k );
}
