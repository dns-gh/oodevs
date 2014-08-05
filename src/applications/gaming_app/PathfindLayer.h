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

#include "clients_gui/EntityLayer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "gaming/Pathfind.h"
#include <tools/Resolver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <boost/optional.hpp>
#include <functional>
#include <deque>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class ModelUnLoaded;
    class Population_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class ModelObserver_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  PathfindLayer
    @brief  Pathfind layer
*/
// Created: LGY 2014-02-28
// =============================================================================
class PathfindLayer : public gui::EntityLayer< kernel::Pathfind_ABC >
                    , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                    , public tools::SelectionObserver_ABC
                    , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                    , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                    , public tools::SelectionObserver_Base< kernel::Formation_ABC >
                    , public tools::SelectionObserver_Base< kernel::Population_ABC >
                    , public tools::SelectionObserver_Base< kernel::Pathfind_ABC >
                    , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                    , public kernel::ContextMenuObserver_ABC< kernel::Pathfind_ABC >
                    , public kernel::OptionsObserver_ABC
                    , private kernel::ModesObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PathfindLayer( kernel::Controllers& controllers,
                            gui::GlTools_ABC& tools,
                            gui::ColorStrategy_ABC& strategy,
                            gui::View_ABC& view,
                            const kernel::Profile_ABC& profile,
                            Publisher_ABC& publisher,
                            const kernel::CoordinateConverter_ABC& converter,
                            const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                            const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                            gui::ModelObserver_ABC& model,
                            actions::ActionsModel& actions );
    virtual ~PathfindLayer();
    //@}

private:
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );

    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( gui::Viewport_ABC& viewport );
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Pathfind_ABC& pathfind, kernel::ContextMenu& menu );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );

    virtual bool HandleKeyPress( QKeyEvent* key );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent( QDragLeaveEvent* event );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );

    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    virtual void Select( const kernel::Pathfind_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void ActivateEntity( const kernel::Entity_ABC& entity );

    void OpenEditingMode( kernel::Entity_ABC* entity, const sword::Pathfind& pathfind );
    void DrawLines( float width ) const;
    void DrawPoints() const;
    void DrawPoint( geometry::Point2f p, bool highlight ) const;
    bool IsNear( float squareDistance, geometry::Point2f point ) const;
    bool PickWaypoint( geometry::Point2f point );
    void PickSegment( geometry::Point2f point );

public slots:
    void OnAcceptEdit();
    void OnRejectEdit();

private slots:
    void ClearPositions();
    void SetStartPosition();
    void SetEndPosition();
    void SendRequest();
    void OnOpenEditingMode();
    void OnDeletePathfind();
    void OnEditPathfind();

private:
    void UpdateHovered( bool snap, const geometry::Point2f& point );
    bool HandleEvent( const std::function< void() >& event, bool replaceable = false );
    void ProcessEvents();
    bool HasValidPathfind() const;
    uint32_t GetUnitId() const;
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    void StopEdition() const;

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::GlTools_ABC& tools_;
    gui::ModelObserver_ABC& model_;
    gui::View_ABC& view_;
    Publisher_ABC& publisher_;
    actions::ActionsModel& actions_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    const tools::Resolver_ABC< kernel::Population_ABC >& populations_;
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f world_;
    // current selection where pathfind is applicable
    kernel::SafePointer< kernel::Entity_ABC > target_;
    // current selected entity
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    // current selected pathfind
    kernel::SafePointer< kernel::Pathfind_ABC > selectedPathfind_;
    // current edited pathfind, kept separate from the model
    std::unique_ptr< Pathfind > edited_;
    // information about the objects under the mouse if not too far from the
    // current path
    boost::optional< Pathfind::Hover > hovered_;
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
    // current tactical lines filter
    kernel::FourStateOption filter_;
    //@}
};

#endif // __PathfindLayer_h_
