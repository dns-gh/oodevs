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
#include <deque>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Agent_ABC;
    class Population_ABC;
    class CoordinateConverter_ABC;
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
                    , public tools::SelectionObserver_ABC
                    , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                    , public tools::SelectionObserver_Base< kernel::Population_ABC >
                    , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                    , public kernel::ModesObserver_ABC
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
    virtual void Paint( gui::Viewport_ABC& viewport );
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );

    virtual bool HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;

    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();

    void DrawLines( float width ) const;
    void DrawPoints() const;
    void DrawPoint( geometry::Point2f p, bool invert ) const;
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
        geometry::Point2f coordinate_;
        boost::optional< uint32_t > waypoint_;
    };

    struct Hover
    {
        geometry::Point2f coordinate_;
        std::size_t lastWaypoint_;
        bool onWaypoint_;
    };

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::GlTools_ABC& tools_;
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    sword::ClientToSim message_;
    kernel::SafePointer< kernel::Entity_ABC > element_;
    std::deque< geometry::Point2f > positions_;
    std::vector< Point > path_;
    boost::optional< Hover > hovered_;
    geometry::Point2f point_;
    bool lock_;
    //@}
};

#endif // __PathfindLayer_h_
