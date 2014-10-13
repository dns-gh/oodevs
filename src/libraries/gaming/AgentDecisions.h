// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentDecisions_h_
#define __AgentDecisions_h_

#include "clients_gui/Decisions.h"
#include "clients_gui/Drawable_ABC.h"

#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class DecisionalModel;
    class Displayer_ABC;
}

namespace sword
{
    class UnitOrder;
    class UnitAttributes;
}

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

// =============================================================================
/** @class  AgentDecisions
    @brief  AgentDecisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AgentDecisions : public gui::Decisions
                     , public gui::Drawable_ABC
                     , public kernel::Displayable_ABC
                     , public kernel::Updatable_ABC< sword::UnitOrder >
                     , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentDecisions( kernel::Controller& controller,
                             const kernel::Agent_ABC& agent,
                             const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver );
    virtual ~AgentDecisions();
    //@}

    //! @name gui::Decisions implementation
    //@{
    virtual bool CanBeOrdered() const;
    //@}

private:
    //! @name kernel::Displayable_ABC implementation
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name kernel::Drawable_ABC implementation
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

    //! @name kernel::Updatatble_ABC implementation
    //@{
    virtual void DoUpdate( const sword::UnitOrder& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver_;
    //@}
};

#endif // __AgentDecisions_h_
