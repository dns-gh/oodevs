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
    class Automat_ABC;
    class Controller;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AutomatDecisions
    @brief  AutomatDecisions
    // $$$$ AGE 2007-04-05: ^c^v de gaming...
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public kernel::Extension_ABC
                       , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( kernel::Controller& controller, const kernel::Automat_ABC& agent );
             AutomatDecisions( xml::xistream& xis, kernel::Controller& controller, const kernel::Automat_ABC& agent );
    virtual ~AutomatDecisions();
    //@}

    //! @name Operations
    //@{
    const kernel::Automat_ABC& GetAgent() const;
    bool CanBeOrdered() const;
    bool IsEmbraye() const;
    void Engage();
    void Disengage();
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatDecisions( const AutomatDecisions& );            //!< Copy constructor
    AutomatDecisions& operator=( const AutomatDecisions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool HasEngagedSuperior() const;
    void ForwardEngageStatus();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Automat_ABC& agent_;
    bool bEmbraye_;
    //@}
};

#endif // __AutomatDecisions_h_
