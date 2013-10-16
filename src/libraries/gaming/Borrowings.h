// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Borrowings_h_
#define __Borrowings_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "Loan.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class EquipmentType;
}

// =============================================================================
/** @class  Borrowings
    @brief  Borrowings
*/
// Created: AGE 2006-02-13
// =============================================================================
class Borrowings : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             Borrowings( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver );
    virtual ~Borrowings();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Borrowings( const Borrowings& );            //!< Copy constructor
    Borrowings& operator=( const Borrowings& ); //!< Assignment operator
    //@}

    typedef std::vector< Loan > T_Borrowings;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver_;
    T_Borrowings borrowings_;
    //@}
};

#endif // __Borrowings_h_
