 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentLogSupply_h_
#define __AgentLogSupply_h_

#include "game_asn/Asn.h"
#include "ModelsContainer.h"
#include "EquipmentAvailability.h"
#include "Dotation.h"

namespace dispatcher
{
class Model;
class Agent;
class Automat;
class Publisher_ABC;

// =============================================================================
/** @class  AgentLogSupply
    @brief  AgentLogSupply
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentLogSupply
{
public:
    //! @name Constructors/Destructor
    //@{
     AgentLogSupply( Model& model, const Agent& agent, const ASN1T_MsgLogSupplyState& asnMsg );
    ~AgentLogSupply();
    //@}

    //! @name Main
    //@{
    void Update(  const ASN1T_MsgLogSupplyState& asnMsg );
    void Send  ( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogSupply( const AgentLogSupply& );            //!< Copy constructor
    AgentLogSupply& operator=( const AgentLogSupply& ); //!< Assignement operator
    //@}

private:
    const Agent&                          agent_;
          Model&                          model_;
          bool                            bSystemEnabled_;
          ModelsContainer< Dotation  >    stocks_;

    ModelsContainer< EquipmentAvailability< ASN1T_LogSupplyEquimentAvailability > > convoyersAvailability_;
};

}

#endif // __AgentLogSupply_h_
