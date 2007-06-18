// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgentList_h_
#define __ActionParameterAgentList_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  ActionParameterAgentList
    @brief  ActionParameterAgentList
*/
// Created: SBO 2007-05-23
// =============================================================================
class ActionParameterAgentList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterAgentList( const kernel::OrderParameter& parameter );
             ActionParameterAgentList( const kernel::OrderParameter& parameter, const ASN1T_UnitList& asn, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
             ActionParameterAgentList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
    virtual ~ActionParameterAgentList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAgentList( const ActionParameterAgentList& );            //!< Copy constructor
    ActionParameterAgentList& operator=( const ActionParameterAgentList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
    //@}
};

#endif // __ActionParameterAgentList_h_
