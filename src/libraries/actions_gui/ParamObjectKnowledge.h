// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObjectKnowledge_h_
#define __ParamObjectKnowledge_h_

#include "EntityParameter.h"

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
/** @class  ParamObjectKnowledge
    @brief  ParamObjectKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledge : public EntityParameter< kernel::ObjectKnowledge_ABC >
                           , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ObjectKnowledgeConverter_ABC* converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

    }
}

#endif // __ParamObjectKnowledge_h_
