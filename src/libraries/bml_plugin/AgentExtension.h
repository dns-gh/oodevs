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
    class Publisher_ABC;
    class ReportFactory;

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-29
// =============================================================================
class AgentExtension : public BmlExtension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitOrder >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Agent& holder, Publisher_ABC& publisher, const ReportFactory& factory  );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
    virtual void DoUpdate( const ASN1T_MsgUnitOrder& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentExtension( const AgentExtension& );            //!< Copy constructor
    AgentExtension& operator=( const AgentExtension& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent& holder_;
    Publisher_ABC& publisher_;
    const ReportFactory& factory_;
    //@}
};

}

#endif // __AgentExtension_h_
