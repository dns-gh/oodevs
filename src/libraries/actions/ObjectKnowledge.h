// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjectKnowledge_h_
#define __ActionParameterObjectKnowledge_h_

#include "Knowledge_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class ObjectKnowledgeId;
}

namespace kernel
{
    class EntityResolver_ABC;
    class Object_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class ObjectKnowledge : public Knowledge_ABC< kernel::ObjectKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             ObjectKnowledge( const kernel::OrderParameter& parameter, unsigned long id, const kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             ObjectKnowledge( const kernel::OrderParameter& parameter, const kernel::ObjectKnowledge_ABC* knowledge, kernel::Controller& controller );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( sword::ObjectKnowledgeId& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}

}

#endif // __ActionParameterObjectKnowledge_h_
