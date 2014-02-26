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

namespace kernel
{
    class AgentType;
    class Agent_ABC;
}

class AgentsModel;

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
             AgentTemplateElement( AgentsModel& agents,
                                   const kernel::Agent_ABC& agent );
             AgentTemplateElement( AgentsModel& agents,
                                   const kernel::AgentType& type,
                                   xml::xistream& xis );
    virtual ~AgentTemplateElement();
    //@}

private:
    //! @name TemplateElement_ABC implementation
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior,
                                             const geometry::Point2f& center,
                                             ColorController& colorController );
    virtual void Serialize( xml::xostream& xos ) const;
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const;
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
