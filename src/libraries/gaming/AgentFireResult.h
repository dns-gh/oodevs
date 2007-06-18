// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFireResult_h_
#define __AgentFireResult_h_

#include "game_asn/Asn.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/Resolver.h"
#include "Casualties.h"

namespace kernel
{
    class Agent_ABC;
    class EquipmentType;
}

class Equipment;

// =============================================================================
/** @class  AgentFireResult
    @brief  AgentFireResult
*/
// Created: AGE 2006-03-10
// =============================================================================
class AgentFireResult : public kernel::Resolver< Equipment >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFireResult( const ASN1T_UnitFireDamages& message, 
                              const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver,
                              const kernel::Resolver_ABC< kernel::EquipmentType >& equipmentResolver );
    virtual ~AgentFireResult();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFireResult( const AgentFireResult& );            //!< Copy constructor
    AgentFireResult& operator=( const AgentFireResult& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& target_;
    Casualties casualties_[ eNbrHumanWound ];
    //@}
};

#endif // __AgentFireResult_h_
