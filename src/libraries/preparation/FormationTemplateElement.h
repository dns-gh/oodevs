// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationTemplateElement_h_
#define __FormationTemplateElement_h_

#include "TemplateElement_ABC.h"

namespace kernel
{
    class Formation_ABC;
}

class FormationModel;

// =============================================================================
/** @class  FormationTemplateElement
    @brief  Formation template element
*/
// Created: AGE 2007-05-29
// =============================================================================
class FormationTemplateElement : public TemplateElement_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationTemplateElement( FormationModel& formations,
                                       const kernel::Formation_ABC& formation );
             FormationTemplateElement( FormationModel& formations,
                                       xml::xistream& xis );
    virtual ~FormationTemplateElement();
    //@}

private:
    //! @name TemplateElement_ABC implementation
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior,
                                             const geometry::Point2f& center,
                                             ColorController& colorController );
    virtual void Serialize( xml::xostream& output ) const;
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    //@}

private:
    //! @name Member data
    //@{
    FormationModel& formations_;
    unsigned int levelId_;
    std::string symbol_;
    //@}
};

#endif // __FormationTemplateElement_h_
