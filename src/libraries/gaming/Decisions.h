// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Decisions_h_
#define __Decisions_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "Decisions_ABC.h"

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace sword
{
    class UnitOrder;
    class UnitAttributes;
}

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
    class DecisionalModel;
}

// =============================================================================
/** @class  Decisions
    @brief  Decisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class Decisions : public kernel::Extension_ABC
                , public kernel::Updatable_ABC< sword::UnitOrder >
                , public kernel::Updatable_ABC< sword::UnitAttributes >
                , public kernel::Displayable_ABC
                , public kernel::Drawable_ABC
                , public Decisions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Decisions( kernel::Controller& controller, const kernel::Agent_ABC& agent,
                     const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver); // $$$$ AGE 2006-04-05: Agent_ABC, pas terrible...
    virtual ~Decisions();
    //@}

    //! @name Operations
    //@{
    virtual bool CanBeOrdered() const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual tools::Iterator< const kernel::Mission& > GetMissions() const;
    virtual tools::Iterator< const kernel::FragOrder& > GetFragOrders() const;
    virtual const kernel::Mission* GetCurrentMission() const;
    virtual const kernel::Entity_ABC& GetAgent() const;

    std::string ModelName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Decisions( const Decisions& );            //!< Copy constructor
    Decisions& operator=( const Decisions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const sword::UnitOrder& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );

    const kernel::DecisionalModel& GetDecisionalModel() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Agent_ABC& agent_;
    const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver_;
    const kernel::Mission* current_;
    const kernel::DecisionalModel* decisionalModel_;
    //@}
};

#endif // __Decisions_h_
