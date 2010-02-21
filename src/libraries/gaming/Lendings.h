// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lendings_h_
#define __Lendings_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "Loan.h"

namespace MsgsSimToClient
{
    class MsgUnitAttributes;
}

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class EquipmentType;
}

// =============================================================================
/** @class  Lendings
    @brief  Lendings
*/
// Created: AGE 2006-02-13
// =============================================================================
class Lendings : public kernel::Extension_ABC
    , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Lendings( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver );
    virtual ~Lendings();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lendings( const Lendings& );            //!< Copy constructor
    Lendings& operator=( const Lendings& ); //!< Assignement operator
    //@}

    typedef std::vector< Loan > T_Lendings;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver_;
    T_Lendings lendings_;
    //@}
};

#endif // __Lendings_h_
