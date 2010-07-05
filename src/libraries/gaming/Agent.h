// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "protocol/simulationsenders.h"

namespace MsgsSimToClient
{
    class MsgUnitCreation;
}

namespace kernel
{
    class Controller;
    class AgentType;
}

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public kernel::EntityImplementation< kernel::Agent_ABC >
            , public kernel::Extension_ABC
            , public kernel::Drawable_ABC
            , public kernel::Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
            Agent( const MsgsSimToClient::MsgUnitCreation& message,
                    kernel::Controller& controller,
                    const tools::Resolver_ABC< kernel::AgentType >& resolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual const kernel::AgentType& GetType() const;
    virtual bool IsCommandPost() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void CreateDictionary( kernel::Controller& controller );
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::AgentType& type_;
    mutable std::string symbol_;
    bool isPc_;
    //@}
};

#endif // __Agent_h_
