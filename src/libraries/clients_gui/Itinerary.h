// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef _gui_Itinerary_h_
#define _gui_Itinerary_h_

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <deque>

namespace sword
{
    class PathResult;
    class Pathfind;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class Viewport_ABC;
    class GLView_ABC;
}

// =============================================================================
/** @class  Itinerary
    @brief  Itinerary
*/
// Created: MCO 2014-08-08
// =============================================================================
class Itinerary : boost::noncopyable
{
public:
    struct Hover
    {
        // current position of the dragged (possibly new) waypoint
        boost::optional< geometry::Point2f > coordinate_;
        // original position of the waypoint being dragged which would
        // only be valid when moving around an already existing one
        boost::optional< geometry::Point2f > origin_;
        // hint describing where to insert a new waypoint
        std::size_t lastWaypoint_;
        // whether the mouse hovers an existing waypoint or not
        bool onWaypoint_;
    };

public:
    Itinerary( const kernel::CoordinateConverter_ABC& converter, const sword::Pathfind& pathfind, bool edition );
    //@}

    void LoadPoints( const sword::PathResult& path );
    void StartOn( const geometry::Point2f& where );
    void StopOn( const geometry::Point2f& where );
    void Clear();
    void Draw( gui::GLView_ABC& view, const boost::optional< Hover >& hover, bool picking ) const;

    bool HasPath() const;
    bool HasWaypoints() const;

    boost::optional< Hover > PickWaypoint( const gui::GLView_ABC& view, const geometry::Point2f& where ) const;
    boost::optional< Hover > PickSegment( const gui::GLView_ABC& view, const geometry::Point2f& where ) const;

    void InsertWaypoint( size_t idx, const geometry::Point2f where );
    void EraseWaypoint( size_t idx );

    std::vector< geometry::Point2f > GetDots() const;
    sword::PathResult GetPathfind() const;
    geometry::Point2f GetPosition() const;

    struct Point
    {
        // position of the point
        geometry::Point2f where;
        // index in the list of waypoints
        boost::optional< uint32_t > waypoint;
    };
    typedef std::deque< Point > T_Points;

private:
    void DrawLines( gui::GLView_ABC& view, const QColor& color, float width, bool picking ) const;
    void DrawPoints( gui::GLView_ABC& view, const QColor& color, const boost::optional< Hover >& hover, bool picking ) const;
    void DrawPoint( gui::GLView_ABC& view, geometry::Point2f p, const QColor& current, bool picking, bool highlight ) const;

private:
    const kernel::CoordinateConverter_ABC& converter_;
    T_Points path_;
    T_Points waypoints_;

};

#endif // __Note_h_
