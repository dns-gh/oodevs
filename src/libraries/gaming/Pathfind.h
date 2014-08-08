// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Pathfind_h_
#define __Pathfind_h_

#include <clients_gui/Drawable_ABC.h>
#include <clients_gui/EntityImplementation.h>
#include <clients_gui/Itinerary.h>
#include <clients_kernel/Pathfind_ABC.h>
#include "tools/Resolver_ABC.h"
#include <boost/optional.hpp>

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class Population_ABC;
}

namespace sword
{
    class PathResult;
}

namespace actions
{
    class ActionsModel;
}

// =============================================================================
/** @class  Note
    @brief  Note
*/
// =============================================================================
class Pathfind : public gui::EntityImplementation< kernel::Pathfind_ABC >
               , public kernel::Extension_ABC
               , public gui::Drawable_ABC
               , public kernel::Updatable_ABC< sword::Pathfind >
{
public:
    //! @name Constructors/Destructor
    //@{
             Pathfind( kernel::Controller& controller,
                       actions::ActionsModel& actionsModel,
                       const kernel::CoordinateConverter_ABC& converter,
                       kernel::Entity_ABC& entity,
                       const sword::Pathfind& msg,
                       bool edition,
                       bool canBeOrdered );
    virtual ~Pathfind();
    //@}

    //! @name gui::Drawable_ABC methods
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Rename( const QString& name );
    virtual void NotifyDestruction() const;
    //@}

    //! @name kernel::Pathfind_ABC methods
    //@{
    virtual const kernel::Entity_ABC& GetUnit() const;
    virtual void SetVisible( bool visible );
    virtual sword::PathResult GetPathfind() const;
    virtual sword::Pathfind GetCreationMessage() const;
    virtual geometry::Point2f GetPosition() const;
    //@}

    //! @name Methods
    //@{
    void LoadPoints( const sword::PathResult& path );
    void Clear();
    void StartOn( const geometry::Point2f& where );
    void StopOn( const geometry::Point2f& where );
    bool HasPath() const;
    bool HasWaypoints() const;
    std::vector< geometry::Point2f > GetDots() const;
    void InsertWaypoint( size_t idx, const geometry::Point2f where );
    void EraseWaypoint( size_t idx );
    boost::optional< Itinerary::Hover > PickWaypoint( const gui::GlTools_ABC& tools, const geometry::Point2f& where ) const;
    boost::optional< Itinerary::Hover > PickSegment( const gui::GlTools_ABC& tools, const geometry::Point2f& where ) const;
    void SetHover( const boost::optional< Itinerary::Hover >& hover );
    //@}

private:
    //! @name helpers
    //@}
    virtual void DoUpdate( const sword::Pathfind& msg );
    //@{

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    actions::ActionsModel& actionsModel_;
    const kernel::CoordinateConverter_ABC& converter_;
    sword::Pathfind pathfind_;
    kernel::Entity_ABC& entity_;
    Itinerary itinerary_;
    boost::optional< Itinerary::Hover > hover_;
    bool visible_;
    //@}

};

#endif // __Note_h_
