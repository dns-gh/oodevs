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

#include "ASN_Types.h"
#include "Types.h"
#include "Resolver.h"
#include "Casualties.h"
class Agent;
class EquipmentType;
class Equipment;

// =============================================================================
/** @class  AgentFireResult
    @brief  AgentFireResult
*/
// Created: AGE 2006-03-10
// =============================================================================
class AgentFireResult : public Resolver< Equipment >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFireResult( const ASN1T_FireDamagesPion& message, 
                              const Resolver_ABC< Agent >& resolver,
                              const Resolver_ABC< EquipmentType >& equipmentResolver );
    virtual ~AgentFireResult();
    //@}

    //! @name Operations
    //@{
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
    const Agent& target_;
    Casualties casualties_[ eNbrHumanWound ];
    //@}
};

#endif // __AgentFireResult_h_
