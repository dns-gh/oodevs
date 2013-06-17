// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPopulationKnowledge_h_
#define __ParamPopulationKnowledge_h_

#include "EntityParameter.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Population_ABC;
    class Entity_ABC;
    class PopulationKnowledge_ABC;
    class AgentKnowledgeConverter_ABC;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamPopulationKnowledge
    @brief  ParamPopulationKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamPopulationKnowledge : public EntityParameter< kernel::PopulationKnowledge_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamPopulationKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamPopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual void Visit( const actions::parameters::PopulationKnowledge& param );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::AgentKnowledgeConverter_ABC* converter_;
    kernel::SafePointer< kernel::Entity_ABC > agent_;
    //@}
};

    }
}

#endif // __ParamPopulationKnowledge_h_
