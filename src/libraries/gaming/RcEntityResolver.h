// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RcEntityResolver_h_
#define __RcEntityResolver_h_

#include "clients_kernel/Resolver.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "RcEntityResolver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Controllers;
}

class ObjectKnowledge_ABC;
class AgentKnowledge_ABC;
class PopulationKnowledge_ABC;

// =============================================================================
/** @class  RcEntityResolver
    @brief  RcEntityResolver
*/
// Created: SBO 2006-09-18
// =============================================================================
class RcEntityResolver : public RcEntityResolver_ABC
                       , public kernel::Resolver< kernel::Agent_ABC >
                       , public kernel::Resolver< ObjectKnowledge_ABC >
                       , public kernel::Resolver< AgentKnowledge_ABC >
                       , public kernel::Resolver< PopulationKnowledge_ABC >
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
                       , public kernel::ElementObserver_ABC< ObjectKnowledge_ABC >
                       , public kernel::ElementObserver_ABC< AgentKnowledge_ABC >
                       , public kernel::ElementObserver_ABC< PopulationKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RcEntityResolver( kernel::Controllers& controllers );
    virtual ~RcEntityResolver();
    //@}

    //! @name Operations
    //@{
    virtual QString CreateLink( const QString& type, unsigned long id ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RcEntityResolver( const RcEntityResolver& );            //!< Copy constructor
    RcEntityResolver& operator=( const RcEntityResolver& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& element );
    virtual void NotifyDeleted( const kernel::Agent_ABC& element );
    virtual void NotifyCreated( const ObjectKnowledge_ABC& element );
    virtual void NotifyDeleted( const ObjectKnowledge_ABC& element );
    virtual void NotifyCreated( const AgentKnowledge_ABC& element );
    virtual void NotifyDeleted( const AgentKnowledge_ABC& element );
    virtual void NotifyCreated( const PopulationKnowledge_ABC& element );
    virtual void NotifyDeleted( const PopulationKnowledge_ABC& element );

    template< typename T >
    QString CreateLink( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __RcEntityResolver_h_
