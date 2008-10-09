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
             ParamObjectKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::Controller& controller );
             ParamObjectKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const kernel::ObjectKnowledge_ABC& potential, kernel::Controller& controller );
    virtual ~ParamObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamObjectKnowledge( const ParamObjectKnowledge& );            //!< Copy constructor
    ParamObjectKnowledge& operator=( const ParamObjectKnowledge& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    kernel::ObjectKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

#endif // __ParamObjectKnowledge_h_
