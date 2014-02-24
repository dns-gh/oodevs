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

#include "TemplateElement_ABC.h"
#include "clients_kernel/Color_ABC.h"
#include <tools/Resolver_ABC.h>
#include <boost/optional/optional.hpp>

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
class AutomatTemplateElement : public TemplateElement_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatTemplateElement( AgentsModel& agents, const kernel::Automat_ABC& automat );
             AutomatTemplateElement( AgentsModel& agents, const tools::Resolver_ABC< kernel::AutomatType, std::string >& types, xml::xistream& input );
    virtual ~AutomatTemplateElement();
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
    AgentsModel& agents_;
    const kernel::AutomatType& type_;
    QString name_;
    boost::optional< kernel::Color_ABC::T_Color > color_;
    std::map< std::string, std::string > extensions_;
    std::string symbol_;
    //@}
};

#endif // __AutomatTemplateElement_h_
