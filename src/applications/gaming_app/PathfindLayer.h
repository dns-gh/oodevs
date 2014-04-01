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
#include <tools/SelectionObserver_ABC.h>
#include <boost/optional.hpp>

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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PathfindLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools,
                            Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& coordinateConverter );
    virtual ~PathfindLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( gui::Viewport_ABC& viewport );
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();

    void DrawLines( float width, uint8_t r, uint8_t g, uint8_t b ) const;
    void DrawPoint( geometry::Point2f p ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void ClearPositions();
    void AddPosition();
    void SendRequest();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::GlTools_ABC& tools_;
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    kernel::SafePointer< kernel::Entity_ABC > element_;
    std::vector< geometry::Point2f > positions_;
    geometry::Point2f point_;
    std::vector< geometry::Point2f > path_;
    //@}
};

#endif // __PathfindLayer_h_
