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

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Iterator.h"

namespace kernel
{
    class Controller;
    class Population_ABC;
    class Mission;
    class FragOrder;
    class DecisionalModel;
}

// =============================================================================
/** @class  PopulationDecisions
    @brief  PopulationDecisions
*/
// Created: AGE 2006-04-10
// =============================================================================
class PopulationDecisions : public kernel::Extension_ABC
                          , public kernel::Updatable_ABC< ASN1T_MsgPopulationOrder >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationDecisions( kernel::Controller& controller, const kernel::Population_ABC& popu );
    virtual ~PopulationDecisions();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Iterator< const kernel::Mission& > GetMissions() const;
    virtual kernel::Iterator< const kernel::FragOrder& > GetFragOrders() const;
    const kernel::Mission* GetCurrentMission() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationDecisions( const PopulationDecisions& );            //!< Copy constructor
    PopulationDecisions& operator=( const PopulationDecisions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationOrder& message );
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
