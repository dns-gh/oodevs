// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObjectKnowledgeList_h_
#define __ParamObjectKnowledgeList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Object_ABC;
    class Entity_ABC;
}

class ObjectKnowledge_ABC;
class ObjectKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamObjectKnowledgeList
    @brief  ParamObjectKnowledgeList
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledgeList : public EntityListParameter< ObjectKnowledge_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledgeList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamObjectKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamObjectKnowledgeList( const ParamObjectKnowledgeList& );            //!< Copy constructor
    ParamObjectKnowledgeList& operator=( const ParamObjectKnowledgeList& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< ObjectKnowledge_ABC >* CreateElement( const ObjectKnowledge_ABC& potential );
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    ObjectKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    unsigned int count_;
    //@}
};

#endif // __ParamObjectKnowledgeList_h_
