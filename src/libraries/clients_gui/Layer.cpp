// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Layer.h"
#include "moc_Layer.cpp"
#include "GLView_ABC.h"
#include "Viewport2d.h"
#include "Viewport3d.h"
#include "clients_kernel/Controllers.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Layer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer::Layer( kernel::Controllers& controllers, GLView_ABC& view, E_LayerTypes type )
    : controllers_( controllers )
    , view_( view )
    , descriptor_( layers::GetDescriptor( type ) )
    , alpha_( 1 )
    , enabled_( true )
    , name_( QString::fromStdString( ENT_Tr::ConvertFromLayerTypes( type ) ) )
{
    SetModes( descriptor_.hiddenModes_, eModes_None, true );
    SetReadOnlyModes( descriptor_.readOnlyModes_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Layer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer::~Layer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Layer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer::Paint( const ViewFrustum& frustum )
{
    Viewport3d viewport( frustum );
    Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: Layer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer::Paint( const geometry::Rectangle2f& rectangle )
{
    if( ShouldDrawPass() )
    {
        Viewport2d viewport( rectangle );
        Paint( viewport );
    }
}

// -----------------------------------------------------------------------------
// Name: Layer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer::Paint( Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer::SetAlpha( float alpha )
{
    alpha_ = alpha;
}

// -----------------------------------------------------------------------------
// Name: Layer::GetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
float Layer::GetAlpha() const
{
    return alpha_;
}

// -----------------------------------------------------------------------------
// Name: Layer::ShouldDrawPass
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
bool Layer::ShouldDrawPass() const
{
    const auto currentPass = view_.GetCurrentPass();
    return IsEnabled()
        && ( descriptor_.passes_.empty() || currentPass.empty() || descriptor_.passes_.find( currentPass ) != std::string::npos )
        && ( !view_.IsPickingMode() || IsPickable() );
}

// -----------------------------------------------------------------------------
// Name: Layer::IsPickable
// Created: LGY 2013-02-25
// -----------------------------------------------------------------------------
bool Layer::IsPickable() const
{
    return descriptor_.pickable_;
}

// -----------------------------------------------------------------------------
// Name: Layer::IsConfigurable
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
bool Layer::IsConfigurable() const
{
    return descriptor_.configurable_;
}

// -----------------------------------------------------------------------------
// Name: Layer::SetVisible
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Layer::SetVisible( bool /*visible*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ForceEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Layer::ForceEnabled( bool enabled )
{
    enabled_ = enabled;
}

// -----------------------------------------------------------------------------
// Name: Layer::EnsureIsEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Layer::EnsureIsEnabled()
{
    enabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: Layer::IsEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
bool Layer::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: Layer::IsVisible
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
bool Layer::IsVisible() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Layer::GetName
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
QString Layer::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Layer::GetOptionName
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
std::string Layer::GetOptionName() const
{
    return ENT_Tr::ConvertFromLayerTypes( descriptor_.type_, ENT_Tr::eToSim );
}

// -----------------------------------------------------------------------------
// Name: Layer::Select
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void Layer::Select( unsigned int, bool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ContextMenu
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void Layer::ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ContextMenu
// Created: LGY 2013-04-03
// -----------------------------------------------------------------------------
bool Layer::ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >&, const geometry::Point2f&, const QPoint& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Layer::FillContextMenu
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
void Layer::FillContextMenu( QMouseEvent* /*mouse*/, kernel::ContextMenu& /*menu*/, const geometry::Point2f& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::FillContextMenu
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
void Layer::FillContextMenu( unsigned int /*id*/, kernel::ContextMenu& /*menu*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ExtractElements
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
void Layer::ExtractElements( T_LayerElements&, const GLView_ABC::T_ObjectsPicking& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ShowTooltip
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
bool Layer::ShowTooltip( const T_ObjectPicking& /*selection*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Layer::HideTooltip
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void Layer::HideTooltip()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::Reset
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
void Layer::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::GetType
// Created: ABR 2014-09-29
// -----------------------------------------------------------------------------
E_LayerTypes Layer::GetType() const
{
    return descriptor_.type_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< E_LayerTypes >& Layer::GetChildrenTypes
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
const std::vector< E_LayerTypes >& Layer::GetChildrenTypes() const
{
    return descriptor_.children_;
}
