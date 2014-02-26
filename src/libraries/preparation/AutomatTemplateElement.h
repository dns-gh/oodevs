// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatTemplateElement_h_
#define __AutomatTemplateElement_h_

#include "TemplateElement.h"

namespace kernel
{
    class Automat_ABC;
    class AutomatType;
}

class AgentsModel;

// =============================================================================
/** @class  AutomatTemplateElement
    @brief  AutomatTemplateElement
*/
// Created: AGE 2007-05-29
// =============================================================================
class AutomatTemplateElement : public TemplateElement
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatTemplateElement( AgentsModel& agents,
                                     const kernel::Automat_ABC& automat );
             AutomatTemplateElement( AgentsModel& agents,
                                     const kernel::AutomatType& type,
                                     xml::xistream& xis );
    virtual ~AutomatTemplateElement();
    //@}

private:
    //! @name TemplateElement implementation
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
    const kernel::AutomatType& type_;
    std::string symbol_;
    //@}
};

#endif // __AutomatTemplateElement_h_
