// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgent_h_
#define __ParamAgent_h_

#include "EntityParameter.h"
#include "clients_kernel/Agent_ABC.h"

namespace kernel
{
    class AgentKnowledge_ABC;
    class Automat_ABC;
    class Formation_ABC;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamAgent
    @brief  ParamAgent
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgent : public EntityParameter< kernel::Agent_ABC >
                 , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                 , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                 , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgent( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamAgent();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddHierarchy( const kernel::Entity_ABC& superior );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& knowledge, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void AddHierarchy();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* superior_;
    //@}
};

    }
}

#endif // __ParamAgent_h_
