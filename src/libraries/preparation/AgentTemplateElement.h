// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentTemplateElement_h_
#define __AgentTemplateElement_h_

#include "TemplateElement.h"

namespace kernel
{
    class AgentType;
    class Agent_ABC;
}

namespace gui
{
    class ColorController_ABC;
}

class AgentsModel;

// =============================================================================
/** @class  AgentTemplateElement
    @brief  AgentTemplateElement
*/
// Created: AGE 2007-05-29
// =============================================================================
class AgentTemplateElement : public TemplateElement
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
    //! @name TemplateElement implementation
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior,
                                             const geometry::Point2f& center,
                                             gui::ColorController_ABC& colorController );
    virtual void Serialize( xml::xostream& xos ) const;
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    //@}

private:
    //! @name Member data
    //@{
    AgentsModel& agents_;
    const kernel::AgentType& type_;
    int level_;
    std::string symbol_;
    const bool cp_;
    //@}
};

#endif // __AgentTemplateElement_h_
