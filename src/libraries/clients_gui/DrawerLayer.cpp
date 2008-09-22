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
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerLayer constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::DrawerLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< Drawing_ABC >( controllers, tools, strategy, view, profile )
    , parameters_( parameters )
    , tools_( tools )
    , selected_( 0 )
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
void DrawerLayer::NotifyContextMenu( const Drawing_ABC& drawing, kernel::ContextMenu& menu )
{
    if( selected_ != &drawing )
        NotifySelected( &drawing );
    menu.InsertItem( "Creation", tools::translate( "DrawerLayer", "Edit drawing..." ), this, SLOT( OnEditDrawing() ) );
    menu.InsertItem( "Creation", tools::translate( "DrawerLayer", "Erase drawing" )  , this, SLOT( OnDeleteDrawing() ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::OnEditDrawing
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::OnEditDrawing()
{
    if( selected_ )
        const_cast< Drawing_ABC* >( selected_ )->Edit( parameters_ );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::OnDeleteDrawing
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void DrawerLayer::OnDeleteDrawing()
{
    delete selected_;
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Paint
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::Paint( const geometry::Rectangle2f& viewport )
{
    viewport_ = viewport;
    Layer_ABC::Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::ShouldDisplay
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
bool DrawerLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::NotifySelected
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::NotifySelected( const Drawing_ABC* selected )
{
    EntityLayer< Drawing_ABC >::NotifySelected( selected );
    selected_ = selected;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Draw
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerLayer::Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& )
{
    static_cast< const Drawing_ABC& >( entity ).Draw( viewport_, tools_, &entity == selected_ );
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
    if( key == Qt::Key_BackSpace || key == Qt::Key_Delete )
    {
        OnDeleteDrawing();
        return true;
    }
    return EntityLayer< Drawing_ABC >::HandleKeyPress( k );
}
