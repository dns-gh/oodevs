// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationDecisions_h_
#define __PopulationDecisions_h_

#include "Decisions_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class Controller;
    class Population_ABC;
    class DecisionalModel;
}

// =============================================================================
/** @class  PopulationDecisions
    @brief  PopulationDecisions
*/
// Created: AGE 2006-04-10
// =============================================================================
class PopulationDecisions : public kernel::Extension_ABC
    , public kernel::Updatable_ABC< Common::MsgPopulationOrder >
                          , public Decisions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationDecisions( kernel::Controller& controller, const kernel::Population_ABC& popu );
    virtual ~PopulationDecisions();
    //@}

    //! @name Operations
    //@{
    virtual bool CanBeOrdered() const;
    virtual tools::Iterator< const kernel::Mission& > GetMissions() const;
    virtual tools::Iterator< const kernel::FragOrder& > GetFragOrders() const;
    virtual const kernel::Mission* GetCurrentMission() const;
    virtual const kernel::Entity_ABC& GetAgent() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationDecisions( const PopulationDecisions& );            //!< Copy constructor
    PopulationDecisions& operator=( const PopulationDecisions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const Common::MsgPopulationOrder& message );
    const kernel::DecisionalModel& GetDecisionalModel() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Population_ABC& popu_;
    const kernel::Mission* current_;
    //@}
};

#endif // __PopulationDecisions_h_
