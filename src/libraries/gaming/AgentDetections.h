// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentDetections_h_
#define __AgentDetections_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class UnitDetection;
}

namespace kernel
{
    class Controller;
    class Agent_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  AgentDetections
    @brief  AgentDetections
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentDetections : public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< sword::UnitDetection >
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                      , public gui::Drawable_ABC
                      , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentDetections( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::Entity_ABC& holder );
    virtual ~AgentDetections();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const kernel::Agent_ABC*, sword::UnitVisibility::Level > T_AgentDetections;
    typedef T_AgentDetections::iterator                                       IT_AgentDetections;
    typedef T_AgentDetections::const_iterator                                CIT_AgentDetections;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitDetection& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const kernel::Entity_ABC& holder_;
    T_AgentDetections detections_;
    //@}
};

#endif // __AgentDetections_h_
