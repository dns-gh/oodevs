// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LightingHelpers.h"

using namespace gui;

namespace
{
    const QRect rect( 0, 0, 48, 48 );
}

QRect lighting::GetEditorRect()
{
    return rect;
}

geometry::Vector3f lighting::PointToDirectionVector( QPoint& position )
{
    const QPoint center = rect.center();
    const QPoint relative = position - center;
    geometry::Vector2f point( 2.f * relative.x() / rect.width(), - 2.f * relative.y() / rect.height() );
    if( point.Length() > 1.f )
        point /= point.Length();
    const float radius = point.Length();
    float angle  = radius > 1e-6 ? std::acos( point.X() / radius ) : 0;
    if( point.Y() < 0 )
        angle = -angle;

    position = center + QPoint( int( 0.5f * point.X() * rect.width() ), int( - 0.5f * point.Y() * rect.height() ) );

    const auto theta = ( 1.f - radius ) * std::acos( -1.f ) * 0.5f;
    geometry::Vector3f direction( 1, 0, 0 );
    direction.Rotate( geometry::Vector3f( 0, 1, 0 ), -theta );
    direction.Rotate( geometry::Vector3f( 0, 0, 1 ), angle );
    return direction;
}
