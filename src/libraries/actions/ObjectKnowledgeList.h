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
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgObjectKnowledgeList;
}

namespace kernel
{
    class Object_ABC;
    class Entity_ABC;
    class ObjectKnowledgeConverter_ABC;
    class Controller;
    class EntityResolver_ABC;
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
             ObjectKnowledgeList( const kernel::OrderParameter& parameter, const Common::MsgObjectKnowledgeList& asn, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             ObjectKnowledgeList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~ObjectKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& asn ) const;
    virtual void Clean( Common::MsgMissionParameter& asn ) const;
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
    void ReadObjectKnowledge( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    //@}
};

    }
}

#endif // __ActionParameterObjectKnowledgeList_h_
