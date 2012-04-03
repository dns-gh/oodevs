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

#include "Casualties.h"
#include "clients_kernel/Types.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class EquipmentType;
}

namespace sword
{
    class UnitFireDamages;
}

class Equipment;

// =============================================================================
/** @class  AgentFireResult
    @brief  AgentFireResult
*/
// Created: AGE 2006-03-10
// =============================================================================
class AgentFireResult : public tools::Resolver< Equipment >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFireResult( const sword::UnitFireDamages& message,
                              const tools::Resolver_ABC< kernel::Agent_ABC >& resolver,
                              const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver,
                              const QDateTime& time );
    virtual ~AgentFireResult();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentFireResult( const AgentFireResult& );            //!< Copy constructor
    AgentFireResult& operator=( const AgentFireResult& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& target_;
    Casualties casualties_[ eNbrHumanWound ];
    QDateTime time_;
    //@}
};

#endif // __AgentFireResult_h_
