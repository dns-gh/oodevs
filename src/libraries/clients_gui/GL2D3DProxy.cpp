// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GL2D3DProxy.h"

#include "EntityLayer.h"
#include "GL3DWidget.h"
#include "GLOptions.h"
#include "GL2D3DProxy.h"
#include "GlTooltip.h"
#include "GL2DWidget.h"
#include "Layer.h"
#include "SvglProxy.h"
#include "TacticalGraphics.h"
#include "TooltipsLayer_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GL2D3DProxy constructor
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
GL2D3DProxy::GL2D3DProxy( GLView_ABC& parent,
                          kernel::Controllers& controllers,
                          const kernel::Profile_ABC& profile,
                          const kernel::StaticModel& staticModel,
                          const kernel::EntityResolver_ABC& model,
                          const std::shared_ptr< Lighting_ABC >& lighting,
                          const kernel::ProfilesModel_ABC& profilesModel,
                          const QString& name,
                          const unsigned id,
                          uint32_t mapnikThread )
    : name_( name )
    , id_( id )
    , parent_( parent )
    , options_( new GLOptions( controllers, profile, staticModel, model, lighting, mapnikThread, profilesModel ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GL2D3DProxy constructor
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
GL2D3DProxy::GL2D3DProxy( GLView_ABC& parent,
                          const GLView_ABC& view,
                          const QString& name,
                          const unsigned id )
    : name_( name )
    , id_( id )
    , parent_( parent )
    , options_( new GLOptions( view.GetActiveOptions() ) )
{
    AddLayers( view.GetLayers() );
}

// -----------------------------------------------------------------------------
// Name: GL2D3DProxy destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GL2D3DProxy::~GL2D3DProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Informations -> implementation
// -----------------------------------------------------------------------------
const QString& GL2D3DProxy::GetName() const
{
    return name_;
}

unsigned GL2D3DProxy::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Proxy -> implementation
// -----------------------------------------------------------------------------
void GL2D3DProxy::Register( const T_View& view )
{
    GLProxyBase::Register( view );
    view->AddLayers( layers_ );
}

void GL2D3DProxy::Unregister( const T_View& view )
{
    GLProxyBase::Unregister( view );
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
        view->RemoveLayer( *it );
}

GLView_ABC& GL2D3DProxy::GetActiveView()
{
    return GetCurrentView();
}

const GLView_ABC& GL2D3DProxy::GetActiveView() const
{
    return GetCurrentView();
}

GLView_ABC& GL2D3DProxy::GetCurrentView()
{
    if( defaultView_ )
        return *defaultView_;
    throw MASA_EXCEPTION( "No default view defined" );
}

const GLView_ABC& GL2D3DProxy::GetCurrentView() const
{
    if( defaultView_ )
        return *defaultView_;
    throw MASA_EXCEPTION( "No default view defined" );
}

GLView_ABC::T_View GL2D3DProxy::GetHoveredView() const
{
    return defaultView_;
}

// -----------------------------------------------------------------------------
// View setters -> forward to parent
// -----------------------------------------------------------------------------
void GL2D3DProxy::SetActiveView( GLView_ABC& )
{
    parent_.SetActiveView( *this );
}

void GL2D3DProxy::SetCurrentView( GLView_ABC* view )
{
    parent_.SetCurrentView( view ? this : 0 );
}

void GL2D3DProxy::SetHoveredView( GLView_ABC* view )
{
    parent_.SetHoveredView( view ? this : 0 );
}

// -----------------------------------------------------------------------------
// Options -> implementation
// -----------------------------------------------------------------------------
void GL2D3DProxy::ApplyOptions()
{
    parent_.ApplyOptions();
}

GLOptions& GL2D3DProxy::GetActiveOptions()
{
    return *options_;
}

const GLOptions& GL2D3DProxy::GetActiveOptions() const
{
    return *options_;
}

GLOptions& GL2D3DProxy::GetCurrentOptions()
{
    return *options_;
}

const GLOptions& GL2D3DProxy::GetCurrentOptions() const
{
    return *options_;
}

// -----------------------------------------------------------------------------
// Layers registration -> implementation & forward to all children
// -----------------------------------------------------------------------------
void GL2D3DProxy::AddLayers( const T_LayersVector& layers )
{
    layers_.insert( layers_.end(), layers.begin(), layers.end() );
    GLProxyBase::AddLayers( layers );
}

void GL2D3DProxy::RemoveLayer( const T_Layer& layer )
{
    auto it = std::find( layers_.begin(), layers_.end(), layer );
    if( it != layers_.end() )
        layers_.erase( it );
    GLProxyBase::RemoveLayer( layer );
}

// -----------------------------------------------------------------------------
// Layers -> implementation
// -----------------------------------------------------------------------------
const T_LayersVector& GL2D3DProxy::GetLayers() const
{
    return layers_;
}

T_Layer GL2D3DProxy::GetLayer( const T_LayerFunctor& functor ) const
{
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
    {
        const auto& layer = *it;
        if( auto subLayer = layer->GetSubLayer( functor ) )
            return subLayer;
        if( functor( layer ) )
            return layer;
    }
    return 0;
}

bool GL2D3DProxy::MoveBelow( const T_Layer& lhs,
                             const T_Layer& rhs )
{
    auto itLhs = std::find( layers_.begin(), layers_.end(), lhs );
    auto itRhs = std::find( layers_.begin(), layers_.end(), rhs );
    if( itLhs == layers_.end() || itRhs == layers_.end() )
        throw MASA_EXCEPTION( "Unable to find layers to move below" );
    if( itLhs < itRhs )
        return false;
    layers_.erase( itLhs );
    layers_.insert( itRhs, lhs );
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->MoveBelow( lhs, rhs );
    return true;
}

void GL2D3DProxy::UpdateLayerOrder()
{
    T_LayersVector orderedLayers;
    std::for_each( layers_.begin(), layers_.end(), [&orderedLayers]( const T_Layer& layer ) {
        if( layer->IsConfigurable() || layer->GetType() == eLayerTypes_RasterDynamic )
            orderedLayers.insert( orderedLayers.begin(), layer );
    } );
    auto& options = GetActiveOptions();
    std::sort( orderedLayers.begin(),
               orderedLayers.end(),
               [ &]( const T_Layer& lhs, const T_Layer& rhs ) -> bool {
        const int lhsPosition = options.Get( "Layers/" + lhs->GetOptionName() + "/Position" ).To< int >();
        const int rhsPosition = options.Get( "Layers/" + rhs->GetOptionName() + "/Position" ).To< int >();
        return lhsPosition < rhsPosition;
    } );
    bool moved = true;
    while( moved )
    {
        moved = false;
        for( auto it = orderedLayers.rbegin(); it != orderedLayers.rend() && it + 1 != orderedLayers.rend(); ++it )
            moved |= MoveBelow( *( it + 1 ), *it );
    }
    for( int i = 0; i < static_cast< int >( orderedLayers.size() ); ++i )
        options.Set( "Layers/" + orderedLayers[ i ]->GetOptionName() + "/Position", i );
}

// -----------------------------------------------------------------------------
// Common picking -> forward to parent
// -----------------------------------------------------------------------------
PickingSelector& GL2D3DProxy::GetPickingSelector()
{
    return parent_.GetPickingSelector();
}

const PickingSelector& GL2D3DProxy::GetPickingSelector() const
{
    return parent_.GetPickingSelector();
}

void GL2D3DProxy::RenderPicking( const T_ObjectPicking& object )
{
    return parent_.RenderPicking( object );
}

// -----------------------------------------------------------------------------
// Tooltip creator -> throw not implemented
// -----------------------------------------------------------------------------
std::unique_ptr< GlTooltip_ABC > GL2D3DProxy::CreateTooltip() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Common drawing operations -> forward to parent
// -----------------------------------------------------------------------------
ColorStrategy_ABC& GL2D3DProxy::GetColorStrategy() const
{
    return parent_.GetColorStrategy();
}

float GL2D3DProxy::GetCurrentAlpha() const
{
    return parent_.GetCurrentAlpha();
}

void GL2D3DProxy::SetCurrentColor( float r, float g, float b, float a )
{
    parent_.SetCurrentColor( r, g, b, a );
}

void GL2D3DProxy::BindIcon( const char** xpm ) const
{
    parent_.BindIcon( xpm );
}

void GL2D3DProxy::DrawActiveFrame() const
{
    parent_.DrawActiveFrame();
}

void GL2D3DProxy::DrawBillboardRect() const
{
    parent_.DrawBillboardRect();
}

void GL2D3DProxy::PrintApp6( const std::string& symbol,
                             const std::string& style,
                             const geometry::Rectangle2f& viewport,
                             unsigned vWidth /* = 640 */,
                             unsigned vHeight /* = 480 */ ) const
{
    parent_.PrintApp6( symbol, style, viewport, vWidth, vHeight );
}

void GL2D3DProxy::DrawSvgInViewport( const std::string& filename,
                                     const geometry::Rectangle2f& viewport,
                                     unsigned vWidth/* = 640 */,
                                     unsigned vHeight /* = 480 */ ) const
{
    parent_.DrawSvgInViewport( filename, viewport, vWidth, vHeight );
}

void GL2D3DProxy::DrawTacticalGraphics( const std::string& symbol,
                                        const kernel::Location_ABC& location,
                                        const geometry::Rectangle2f& viewport,
                                        bool overlined,
                                        float pointSize,
                                        float zoom ) const
{
    parent_.DrawTacticalGraphics( symbol, location, viewport, overlined, pointSize, zoom );
}
