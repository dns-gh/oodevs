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
#include "clients_kernel/Population_ABC.h"

namespace kernel
{
    class Population_ABC;
    class Entity_ABC;
    class PopulationKnowledge_ABC;
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
class ParamPopulationKnowledge : public EntityParameter< kernel::Population_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::PopulationKnowledge_ABC >
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
    virtual void Visit( const actions::parameters::PopulationKnowledge& param );
    virtual void NotifyContextMenu( const kernel::PopulationKnowledge_ABC& knowledge, kernel::ContextMenu& menu );
    //@}
};

    }
}

#endif // __ParamPopulationKnowledge_h_
