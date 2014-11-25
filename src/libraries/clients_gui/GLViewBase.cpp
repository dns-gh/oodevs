// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLViewBase.h"

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GLViewBase constructor
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
GLViewBase::GLViewBase( GLView_ABC& parent )
    : parent_( parent )
    , context_( new QGLContext( QGLFormat::defaultFormat() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GLViewBase destructor
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
GLViewBase::~GLViewBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Information -> forward to parent
// -----------------------------------------------------------------------------
const QString& GLViewBase::GetName() const
{
    return parent_.GetName();
}

unsigned GLViewBase::GetID() const
{
    return parent_.GetID();
}

// -----------------------------------------------------------------------------
// Proxy -> throw not implemented
// -----------------------------------------------------------------------------
void GLViewBase::Register( const std::shared_ptr< GLView_ABC >& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

void GLViewBase::Unregister( const std::shared_ptr< GLView_ABC >& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

GLView_ABC& GLViewBase::GetCurrentView()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

const GLView_ABC& GLViewBase::GetCurrentView() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

GLView_ABC& GLViewBase::GetActiveView()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

const GLView_ABC& GLViewBase::GetActiveView() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

GLView_ABC::T_View GLViewBase::GetHoveredView() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// View setters -> forward to parent
// -----------------------------------------------------------------------------
void GLViewBase::SetActiveView( GLView_ABC& )
{
    parent_.SetActiveView( *this );
}

void GLViewBase::SetCurrentView( GLView_ABC* view )
{
    parent_.SetCurrentView( view ? this : 0 );
}

void GLViewBase::SetHoveredView( GLView_ABC* view )
{
    parent_.SetHoveredView( view ? this : 0 );
}

// -----------------------------------------------------------------------------
// Options -> forward to parent
// -----------------------------------------------------------------------------
void GLViewBase::ApplyOptions()
{
    parent_.ApplyOptions();
}

GLOptions& GLViewBase::GetActiveOptions()
{
    return parent_.GetActiveOptions();
}

const GLOptions& GLViewBase::GetActiveOptions() const
{
    return parent_.GetActiveOptions();
}

GLOptions& GLViewBase::GetCurrentOptions()
{
    return parent_.GetCurrentOptions();
}

const GLOptions& GLViewBase::GetCurrentOptions() const
{
    return parent_.GetCurrentOptions();
}

// -----------------------------------------------------------------------------
// Layers helpers -> throw not implemented
// -----------------------------------------------------------------------------
const T_LayersVector& GLViewBase::GetLayers() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

bool GLViewBase::IsInAReadOnlyLayer( const kernel::GraphicalEntity_ABC& ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

void GLViewBase::UpdateLayerOrder()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Picking -> forward to parent
// -----------------------------------------------------------------------------
PickingSelector& GLViewBase::GetPickingSelector()
{
    return parent_.GetPickingSelector();
}

const PickingSelector& GLViewBase::GetPickingSelector() const
{
    return parent_.GetPickingSelector();
}

void GLViewBase::RenderPicking( const T_ObjectPicking& object )
{
    parent_.RenderPicking( object );
}

// -----------------------------------------------------------------------------
// Tooltip creator -> throw not implemented
// -----------------------------------------------------------------------------
std::unique_ptr< GlTooltip_ABC > GLViewBase::CreateTooltip() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Common drawing operations -> forward to parent
// -----------------------------------------------------------------------------
ColorStrategy_ABC& GLViewBase::GetColorStrategy() const
{
    return parent_.GetColorStrategy();
}

float GLViewBase::GetCurrentAlpha() const
{
    return parent_.GetCurrentAlpha();
}

void GLViewBase::SetCurrentColor( float r, float g, float b, float a )
{
    parent_.SetCurrentColor( r, g, b, a );
}

void GLViewBase::BindIcon( const char** xpm ) const
{
    parent_.BindIcon( xpm );
}

void GLViewBase::DrawActiveFrame() const
{
    parent_.DrawActiveFrame();
}

void GLViewBase::DrawBillboardRect() const
{
    parent_.DrawBillboardRect();
}

void GLViewBase::PrintApp6( const std::string& symbol,
                            const std::string& style,
                            const geometry::Rectangle2f& viewport,
                            unsigned vWidth /* = 640 */,
                            unsigned vHeight /* = 480 */ ) const
{
    parent_.PrintApp6( symbol, style, viewport, vWidth, vHeight );
}

void GLViewBase::DrawSvgInViewport( const std::string& filename,
                                    const geometry::Rectangle2f& viewport,
                                    unsigned vWidth/* = 640 */,
                                    unsigned vHeight /* = 480 */ ) const
{
    parent_.DrawSvgInViewport( filename, viewport, vWidth, vHeight );
}

void GLViewBase::DrawTacticalGraphics( const std::string& symbol,
                                       const kernel::Location_ABC& location,
                                       const geometry::Rectangle2f& viewport,
                                       bool overlined,
                                       float pointSize,
                                       float zoom ) const
{
    parent_.DrawTacticalGraphics( symbol, location, viewport, overlined, pointSize, zoom );
}
