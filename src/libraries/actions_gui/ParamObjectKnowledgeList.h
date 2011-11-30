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
    class ObjectKnowledge_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamObjectKnowledgeList
    @brief  ParamObjectKnowledgeList
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledgeList : public EntityListParameter< kernel::ObjectKnowledge_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledgeList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller,
                                       kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamObjectKnowledgeList();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::ObjectKnowledge_ABC >* CreateElement( const kernel::ObjectKnowledge_ABC& potential );
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ObjectKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    unsigned int count_;
    //@}
};
}
}

#endif // __ParamObjectKnowledgeList_h_
