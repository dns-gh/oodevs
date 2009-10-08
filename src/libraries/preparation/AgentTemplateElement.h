// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentTemplateElement_h_
#define __AgentTemplateElement_h_

#include "TemplateElement_ABC.h"
#include "tools/Resolver_ABC.h"
class AgentsModel;

namespace kernel
{
    class AgentType;
    class Agent_ABC;
}

// =============================================================================
/** @class  AgentTemplateElement
    @brief  AgentTemplateElement
*/
// Created: AGE 2007-05-29
// =============================================================================
class AgentTemplateElement : public TemplateElement_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentTemplateElement( AgentsModel& agents, const kernel::Agent_ABC& agent );
             AgentTemplateElement( AgentsModel& agents, const tools::Resolver_ABC< kernel::AgentType, std::string >& types, xml::xistream& input );
    virtual ~AgentTemplateElement();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center );
    virtual void Serialize( xml::xostream& output );
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    virtual QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentTemplateElement( const AgentTemplateElement& );            //!< Copy constructor
    AgentTemplateElement& operator=( const AgentTemplateElement& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    AgentsModel& agents_;
    const kernel::AgentType& type_;
    bool cp_;
    //@}
};

#endif // __AgentTemplateElement_h_
