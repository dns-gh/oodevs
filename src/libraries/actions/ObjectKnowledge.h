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

#include "Entity.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Object_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class ObjectKnowledge : public Entity< kernel::ObjectKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             ObjectKnowledge( const kernel::OrderParameter& parameter, unsigned long id, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             ObjectKnowledge( xml::xistream& xis, const tools::Resolver_ABC< kernel::Object_ABC >& resolver, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             ObjectKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::Object_ABC >& resolver, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_ObjectKnowledge& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

    }
}

#endif // __ActionParameterObjectKnowledge_h_
