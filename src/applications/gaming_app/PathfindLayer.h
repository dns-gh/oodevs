// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __PathfindLayer_h_
#define __PathfindLayer_h_

#include "clients_gui/Layer.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include <tools/SelectionObserver_ABC.h>
#include <boost/optional.hpp>
#include <functional>
#include <deque>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Agent_ABC;
    class Population_ABC;
    class CoordinateConverter_ABC;
    class ModelUnLoaded;
}

namespace gui
{
    class GlTools_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  PathfindLayer
    @brief  Pathfind layer
*/
// Created: LGY 2014-02-28
// =============================================================================
class PathfindLayer : public gui::Layer
                    , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                    , public tools::SelectionObserver_ABC
                    , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                    , public tools::SelectionObserver_Base< kernel::Population_ABC >
                    , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                    , private kernel::ModesObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PathfindLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools,
                            Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PathfindLayer();
    //@}

private:
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );

    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( gui::Viewport_ABC& viewport );
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );

    virtual bool HandleKeyPress( QKeyEvent* key );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent( QDragLeaveEvent* event );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );

    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();

    void DrawLines( float width ) const;
    void DrawPoints() const;
    void DrawPoint( geometry::Point2f p, bool highlight ) const;
    bool IsNear( float squareDistance, geometry::Point2f point ) const;
    bool PickWaypoint( geometry::Point2f point );
    void PickSegment( geometry::Point2f point );

private slots:
    void ClearPositions();
    void SetStartPosition();
    void SetEndPosition();
    void SendRequest();
    void OpenEditingMode();

private:
    struct Point
    {
        // position of the point
        geometry::Point2f coordinate_;
        // index in the list of waypoints
        boost::optional< uint32_t > waypoint_;
    };

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

private:
    void UpdateHovered( bool snap, const geometry::Point2f& point );
    bool HandleEvent( const std::function< void() >& event, bool replaceable = false );
    void ProcessEvents();

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::GlTools_ABC& tools_;
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f world_;
    kernel::SafePointer< kernel::Entity_ABC > element_;
    // the waypoints including the one being currently added or dragged
    std::deque< geometry::Point2f > positions_;
    // the last received path
    std::vector< Point > path_;
    // information about the objects under the mouse if not too far from the
    // current path
    boost::optional< Hover > hovered_;
    geometry::Point2f point_;
    // pending events stored while waiting for a reply to the previous request
    // from the server
    std::deque< std::function< void() > > events_;
    // whether the last event stored can be overwritten or must be preserved,
    // typically only the last move event is being kept in order to reduce
    // message load
    bool replaceable_;
    // whether a request has been sent to the server and a reply is expected
    // before letting the user make more changes to the displayed path
    bool lock_;
    //@}
};

#endif // __PathfindLayer_h_
