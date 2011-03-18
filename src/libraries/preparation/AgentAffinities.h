// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AgentAffinities_h_
#define __AgentAffinities_h_

#include "Affinities.h"
#include "Types.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
}


// =============================================================================
/** @class  AgentAffinities
    @brief  Agent affinities
*/
// Created: LGY 2011-03-14
// =============================================================================
class AgentAffinities : public Affinities
                      , public tools::Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentAffinities( kernel::Controllers& controllers );
             AgentAffinities( xml::xistream& xis, kernel::Controllers& controllers );
    virtual ~AgentAffinities();
    //@}

    //! @name Operations
    //@{
    virtual void Clear();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentAffinities( const AgentAffinities& );            //!< Copy constructor
    AgentAffinities& operator=( const AgentAffinities& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __AgentAffinities_h_
