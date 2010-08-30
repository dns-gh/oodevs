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

#include "Parameter.h"

namespace Common
{
    class UnitIdList;
}

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class EntityResolver_ABC;
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
             AgentList( const kernel::OrderParameter& parameter, const Common::UnitIdList& message, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
             AgentList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~AgentList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;
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
    void ReadAgent( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    //@}
};

    }
}

#endif // __ActionParameterAgentList_h_
