// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentExtension_h_
#define __AgentExtension_h_

#include "BmlExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Agent;
}

namespace bml
{

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-29
// =============================================================================
class AgentExtension : public BmlExtension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentExtension( dispatcher::Agent& holder );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentExtension( const AgentExtension& );            //!< Copy constructor
    AgentExtension& operator=( const AgentExtension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateWhere( const ASN1T_MsgUnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent& holder_;
    //@}
};

}

#endif // __AgentExtension_h_
