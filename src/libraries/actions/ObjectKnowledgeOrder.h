// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjectKnowledgeOrder_h_
#define __ActionParameterObjectKnowledgeOrder_h_

#include "ObjectKnowledge.h"

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  ObjectKnowledgeOrder
    @brief  Object knowledge order
*/
// Created: LGY 2011-07-07
// =============================================================================
class ObjectKnowledgeOrder : public ObjectKnowledge
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                   const kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~ObjectKnowledgeOrder();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool CheckKnowledgeValidity() const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Member Data
    //@{
    const kernel::ObjectKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& owner_;
    const kernel::Object_ABC& object_;
    //@}
};

}

}

#endif // __ActionParameterObjectKnowledgeOrder_h_
