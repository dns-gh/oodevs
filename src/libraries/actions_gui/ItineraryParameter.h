// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ItineraryParameter_h
#define ItineraryParameter_h

#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Pathfind_ABC;
}

namespace sword
{
    class Pathfind;
}

namespace actions
{
namespace gui
{
    class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ItineraryParameter
    @brief  ItineraryParameter
*/
// Created: ABR 2014-05-16
// =============================================================================
class ItineraryParameter : public Param_ABC
                         , public kernel::ContextMenuObserver_ABC< kernel::Pathfind_ABC >
                         , public tools::ElementObserver_ABC< kernel::Pathfind_ABC >
{
public:
             ItineraryParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ItineraryParameter();

    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::Itinerary& param );
    
private:
    virtual void OnMenuClick();

    virtual bool InternalCheckValidity() const;
    virtual void NotifyContextMenu( const kernel::Pathfind_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const kernel::Pathfind_ABC& entity );
    virtual void NotifyDeleted( const kernel::Pathfind_ABC& entity );

    void Display( const QString& what );

    sword::Pathfind GetPathfind() const;

private:
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const tools::Resolver< kernel::Pathfind_ABC >& resolver_;
    std::unique_ptr< sword::Pathfind > pathfind_;
    QLabel* entityLabel_;
    const kernel::Pathfind_ABC* potential_;
    const kernel::Pathfind_ABC* selected_;
};

}
}

#endif // ItineraryParameter_h
