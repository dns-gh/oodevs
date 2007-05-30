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

class FormationModel;
namespace kernel
{
    class Formation_ABC;
}

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
             FormationTemplateElement( FormationModel& formations, const kernel::Formation_ABC& formation );
             FormationTemplateElement( FormationModel& formations, xml::xistream& input );
    virtual ~FormationTemplateElement();
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
    FormationTemplateElement( const FormationTemplateElement& );            //!< Copy constructor
    FormationTemplateElement& operator=( const FormationTemplateElement& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    FormationModel& formations_;
    unsigned int levelId_;
    QString name_;
    //@}
};

#endif // __FormationTemplateElement_h_
