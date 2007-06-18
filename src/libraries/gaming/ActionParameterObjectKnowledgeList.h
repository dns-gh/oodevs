// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjectKnowledgeList_h_
#define __ActionParameterObjectKnowledgeList_h_

#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Object_ABC;
}

class ObjectKnowledgeConverter_ABC;

// =============================================================================
/** @class  ActionParameterObjectKnowledgeList
    @brief  ActionParameterObjectKnowledgeList
*/
// Created: SBO 2007-05-24
// =============================================================================
class ActionParameterObjectKnowledgeList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterObjectKnowledgeList( const kernel::OrderParameter& parameter );
             ActionParameterObjectKnowledgeList( const kernel::OrderParameter& parameter, const ASN1T_ObjectKnowledgeList& asn, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterObjectKnowledgeList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~ActionParameterObjectKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObjectKnowledgeList( const ActionParameterObjectKnowledgeList& );            //!< Copy constructor
    ActionParameterObjectKnowledgeList& operator=( const ActionParameterObjectKnowledgeList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadObjectKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    //@}
};

#endif // __ActionParameterObjectKnowledgeList_h_
