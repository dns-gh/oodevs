// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentList_h_
#define __ParamAgentList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class OrderParameter;
}

// =============================================================================
/** @class  ParamAgentList
    @brief  ParamAgentList
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAgentList : public EntityListParameter< kernel::Agent_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamAgentList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void AddHierarchy();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgentList( const ParamAgentList& );            //!< Copy constructor
    ParamAgentList& operator=( const ParamAgentList& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Agent_ABC >* CreateElement( const kernel::Agent_ABC& potential );
    void AddHierarchy( const kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    unsigned int count_;
    const kernel::Entity_ABC* superior_; // $$$$ AGE 2007-10-15: deletions !
    //@}
};

#endif // __ParamAgentList_h_
