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

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Iterator.h"

namespace kernel
{
    class Population_ABC;
    class Mission;
}

// =============================================================================
/** @class  PopulationDecisions
    @brief  PopulationDecisions
    // $$$$ AGE 2006-06-29: Current mission !
*/
// Created: AGE 2006-04-10
// =============================================================================
class PopulationDecisions : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationDecisions( const kernel::Population_ABC& popu );
    virtual ~PopulationDecisions();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Iterator< const kernel::Mission& > GetMissions() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationDecisions( const PopulationDecisions& );            //!< Copy constructor
    PopulationDecisions& operator=( const PopulationDecisions& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Population_ABC& popu_;
    //@}
};

#endif // __PopulationDecisions_h_
