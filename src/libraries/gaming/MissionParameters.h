// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameters_h_
#define __MissionParameters_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/Resolver.h>

namespace kernel
{
    class Controller;
    class OrderType;
}

namespace actions
{
    class Action_ABC;
    class ActionFactory_ABC;
}

namespace sword
{
    class UnitOrder;
    class AutomatOrder;
    class CrowdOrder;
    class FragOrder;
    class TaskCreationRequestAck;
    class FragOrderAck;
}

// =============================================================================
/** @class  MissionParameters
    @brief  Mission parameters
*/
// Created: SBO 2006-11-13
// =============================================================================
class MissionParameters : public kernel::Extension_ABC
                        , public tools::Observer_ABC
                        , public kernel::Updatable_ABC< sword::UnitOrder >
                        , public kernel::Updatable_ABC< sword::AutomatOrder >
                        , public kernel::Updatable_ABC< sword::CrowdOrder >
                        , public kernel::Updatable_ABC< sword::FragOrder >
                        , public kernel::Updatable_ABC< sword::TaskCreationRequestAck >
                        , public kernel::Updatable_ABC< sword::FragOrderAck >
                        , public tools::Resolver< actions::Action_ABC >
                        , public tools::ElementObserver_ABC< actions::Action_ABC >
                        , public gui::Drawable_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionParameters( kernel::Controller& controller, const actions::ActionFactory_ABC& factory, unsigned long entityId );
    virtual ~MissionParameters();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void NotifyCreated( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const actions::Action_ABC& action );
    void Display( kernel::Displayer_ABC& displayer ) const;
    unsigned long GetEntityId() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitOrder& message );
    virtual void DoUpdate( const sword::AutomatOrder& message );
    virtual void DoUpdate( const sword::CrowdOrder& message );
    virtual void DoUpdate( const sword::FragOrder& message );
    virtual void DoUpdate( const sword::TaskCreationRequestAck& message );
    virtual void DoUpdate( const sword::FragOrderAck& message );
    template< typename T >
    void UpdateMessage( const T& message );
    template< typename T >
    void UpdateMessageAck( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const actions::ActionFactory_ABC& factory_;
    unsigned long entityId_;
    const kernel::OrderType* currentMission_;
    //@}
};

#endif // __MissionParameters_h_
