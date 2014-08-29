// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PickingSelector.h"
#include <boost/optional.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PickingSelector constructor
// Created: LGY 2013-03-13
// -----------------------------------------------------------------------------
PickingSelector::PickingSelector()
    : pickingMode_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PickingSelector destructor
// Created: LGY 2013-03-13
// -----------------------------------------------------------------------------
PickingSelector::~PickingSelector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PickingSelector::FillSelection
// Created: LGY 2013-03-14
// -----------------------------------------------------------------------------
void PickingSelector::FillSelection( GlTools_ABC::T_ObjectsPicking& selection,
        const boost::optional< E_LayerTypes >& types, const std::function< void() >& paint )
{
    if( types )
        pickingLayers_.insert( *types );
    pickingMode_ = true;
    paint();
    selection = pickObjects_;
    renderObjects_.clear();
    pickObjects_.clear();
    pickingMode_ = false;
    pickingLayers_.clear();
}

// -----------------------------------------------------------------------------
// Name: PickingSelector::Picking
// Created: LGY 2013-03-14
// -----------------------------------------------------------------------------
void PickingSelector::Picking( QPoint point, unsigned int windowHeight )
{
    unsigned char pixel[ 3 ];
    glReadPixels( point.x(), windowHeight - point.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel );
    if( !renderObjects_.empty() )
    {
        T_RenderObject lastObject = renderObjects_.back();
        if( std::get< 0 >( lastObject ) == (int) pixel[ 0 ] &&
            std::get< 1 >( lastObject ) == (int) pixel[ 1 ] &&
            std::get< 2 >( lastObject ) == (int) pixel[ 2 ] )
        {
            pickObjects_.push_back( std::get< 3 >( lastObject ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PickingSelector::RenderPicking
// Created: LGY 2013-03-14
// -----------------------------------------------------------------------------
void PickingSelector::RenderPicking( const GlTools_ABC::T_ObjectPicking& object, std::function< void( float, float, float, float ) > setCurrentColor )
{
    int red = std::rand() % 256;
    int green = std::rand() % 256;
    int blue = std::rand() % 256;
    color_ = QColor( red, green, blue );
    setCurrentColor( red / 255.f, green / 255.f, blue / 255.f, 1.f );
    renderObjects_.push_back( std::make_tuple( red, green, blue, object ) );
}

// -----------------------------------------------------------------------------
// Name: PickingSelector::GetColor
// Created: LGY 2013-03-15
// -----------------------------------------------------------------------------
QColor PickingSelector::GetColor() const
{
    return color_;
}

// -----------------------------------------------------------------------------
// Name: PickingSelector::IsPickingMode
// Created: LGY 2013-03-14
// -----------------------------------------------------------------------------
bool PickingSelector::IsPickingMode() const
{
    return pickingMode_;
}

// -----------------------------------------------------------------------------
// Name: PickingSelector::ShouldDisplay
// Created: LGY 2013-03-14
// -----------------------------------------------------------------------------
bool PickingSelector::ShouldDisplay( E_LayerTypes type ) const
{
    return pickingLayers_.empty() || pickingLayers_.find( type ) != pickingLayers_.end();
}
