// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatDecisions_h_
#define __AutomatDecisions_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
}

// =============================================================================
/** @class  AutomatDecisions
    @brief  AutomatDecisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public kernel::Extension_ABC
                       , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( kernel::Controller& controller, const kernel::Agent_ABC& agent );
    virtual ~AutomatDecisions();
    //@}

    //! @name Operations
    //@{
    const kernel::Agent_ABC& GetAgent() const;
    bool IsEmbraye() const; // $$$$ AGE 2006-03-14: 
    void Engage();
    void Disengage();
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatDecisions( const AutomatDecisions& );            //!< Copy constructor
    AutomatDecisions& operator=( const AutomatDecisions& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Agent_ABC& agent_;
    bool bEmbraye_;
    //@}
};

#endif // __AutomatDecisions_h_
