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

#include "ObjectKnowledge_ABC.h"
#include "ActionParameterEntity.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Object_ABC;
}

class ObjectKnowledgeConverter_ABC;

// =============================================================================
/** @class  ActionParameterObjectKnowledge
    @brief  ActionParameterObjectKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class ActionParameterObjectKnowledge : public ActionParameterEntity< ObjectKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterObjectKnowledge( const QString& name );
    explicit ActionParameterObjectKnowledge( const kernel::OrderParameter& parameter );
             ActionParameterObjectKnowledge( const kernel::OrderParameter& parameter, unsigned long id, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterObjectKnowledge( const QString& name, unsigned long id, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterObjectKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterObjectKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~ActionParameterObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_KnowledgeObject& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObjectKnowledge( const ActionParameterObjectKnowledge& );            //!< Copy constructor
    ActionParameterObjectKnowledge& operator=( const ActionParameterObjectKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterObjectKnowledge_h_
