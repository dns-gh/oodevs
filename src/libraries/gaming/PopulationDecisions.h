// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationDecisions_h_
#define __PopulationDecisions_h_

#include "clients_gui/Decisions.h"

#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Population_ABC;
    class PopulationType;
    class DecisionalModel;
}

namespace sword
{
    class CrowdOrder;
    class CrowdUpdate;
}

// =============================================================================
/** @class  PopulationDecisions
    @brief  PopulationDecisions
*/
// Created: AGE 2006-04-10
// =============================================================================
class PopulationDecisions : public gui::Decisions
                          , public kernel::Updatable_ABC< sword::CrowdOrder >
                          , public kernel::Updatable_ABC< sword::CrowdUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationDecisions( kernel::Controller& controller,
                                  const kernel::Population_ABC& population,
                                  const kernel::PopulationType& type );
    virtual ~PopulationDecisions();
    //@}

    //! @name Operations
    //@{
    virtual bool CanBeOrdered() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::CrowdOrder& message );
    virtual void DoUpdate( const sword::CrowdUpdate& message );
    //@}
};

#endif // __PopulationDecisions_h_
