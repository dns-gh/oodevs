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
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"

namespace kernel
{
    class Object_ABC;
    class Entity_ABC;
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
    virtual void Purge();
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual void Visit( const actions::parameters::ObjectKnowledge& param );
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
    kernel::SafePointer< kernel::Entity_ABC > agent_;
    //@}
};

    }
}

#endif // __ParamObjectKnowledge_h_
