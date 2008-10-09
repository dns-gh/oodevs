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

#include "game_asn/Simulation.h"
#include "Parameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  AgentList
    @brief  AgentList
*/
// Created: SBO 2007-05-23
// =============================================================================
class AgentList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentList( const kernel::OrderParameter& parameter );
             AgentList( const kernel::OrderParameter& parameter, const ASN1T_UnitList& asn, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::Controller& controller );
             AgentList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::Controller& controller );
    virtual ~AgentList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentList( const AgentList& );            //!< Copy constructor
    AgentList& operator=( const AgentList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::Controller& controller );
    //@}
};

    }
}

#endif // __ActionParameterAgentList_h_
