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
#include "clients_kernel/Color_ABC.h"
#include <boost/optional/optional.hpp>

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
             FormationTemplateElement( FormationModel& formations, const kernel::Formation_ABC& formation );
             FormationTemplateElement( FormationModel& formations, xml::xistream& input );
    virtual ~FormationTemplateElement();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center, ColorController& colorController );
    virtual void Serialize( xml::xostream& output );
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    virtual QString GetName() const;
    virtual void Rename( const QString& name );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadExtension( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    FormationModel& formations_;
    unsigned int levelId_;
    QString name_;
    boost::optional< kernel::Color_ABC::T_Color > color_;
    std::map< std::string, std::string > extensions_;
    std::string symbol_;
    //@}
};

#endif // __FormationTemplateElement_h_
