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

#include "Parameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Object_ABC;
    class Entity_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObjectKnowledgeList
    @brief  ObjectKnowledgeList
*/
// Created: SBO 2007-05-24
// =============================================================================
class ObjectKnowledgeList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectKnowledgeList( const kernel::OrderParameter& parameter );
             ObjectKnowledgeList( const kernel::OrderParameter& parameter, const ASN1T_ObjectKnowledgeList& asn, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ObjectKnowledgeList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Object_ABC >& resolver, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~ObjectKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledgeList( const ObjectKnowledgeList& );            //!< Copy constructor
    ObjectKnowledgeList& operator=( const ObjectKnowledgeList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadObjectKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Object_ABC >& resolver, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    //@}
};

    }
}

#endif // __ActionParameterObjectKnowledgeList_h_
