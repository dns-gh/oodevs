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

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

namespace MsgsSimToClient
{
    class MsgUnitDetection;
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
                      , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitDetection >
                      , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentDetections( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::Entity_ABC& holder );
    virtual ~AgentDetections();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentDetections( const AgentDetections& );            //!< Copy constructor
    AgentDetections& operator=( const AgentDetections& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Agent_ABC*, EnumUnitVisibility > T_AgentDetections;
    typedef T_AgentDetections::const_iterator                            CIT_AgentDetections;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitDetection& message );
    bool IsSameTeam( const kernel::Entity_ABC& entity ) const;
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
