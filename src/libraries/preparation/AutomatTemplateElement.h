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
#include "clients_kernel/Resolver_ABC.h"

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
             AutomatTemplateElement( AgentsModel& agents, const kernel::Resolver_ABC< kernel::AutomatType, std::string >& types, xml::xistream& input );
    virtual ~AutomatTemplateElement();
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
    AutomatTemplateElement( const AutomatTemplateElement& );            //!< Copy constructor
    AutomatTemplateElement& operator=( const AutomatTemplateElement& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    AgentsModel& agents_;
    const kernel::AutomatType& type_;
    //@}
};

#endif // __AutomatTemplateElement_h_
