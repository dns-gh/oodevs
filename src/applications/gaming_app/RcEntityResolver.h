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

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
#include "reports/RcEntityResolver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Object_ABC;
    class ObjectKnowledge_ABC;
    class AgentKnowledge_ABC;
    class PopulationKnowledge_ABC;
}

// =============================================================================
/** @class  RcEntityResolver
    @brief  RcEntityResolver
*/
// Created: SBO 2006-09-18
// =============================================================================
class RcEntityResolver : public RcEntityResolver_ABC
                       , public tools::Resolver< kernel::Agent_ABC >
                       , public tools::Resolver< kernel::Automat_ABC >
                       , public tools::Resolver< kernel::ObjectKnowledge_ABC >
                       , public tools::Resolver< kernel::Object_ABC >
                       , public tools::Resolver< kernel::AgentKnowledge_ABC >
                       , public tools::Resolver< kernel::PopulationKnowledge_ABC >
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                       , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                       , public tools::ElementObserver_ABC< kernel::ObjectKnowledge_ABC >
                       , public tools::ElementObserver_ABC< kernel::Object_ABC >
                       , public tools::ElementObserver_ABC< kernel::AgentKnowledge_ABC >
                       , public tools::ElementObserver_ABC< kernel::PopulationKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             RcEntityResolver( kernel::Controllers& controllers );
    virtual ~RcEntityResolver();
    //@}

    //! @name Operations
    //@{
    virtual QString CreateLink( const QString& type, unsigned long id ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RcEntityResolver( const RcEntityResolver& );            //!< Copy constructor
    RcEntityResolver& operator=( const RcEntityResolver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& element );
    virtual void NotifyDeleted( const kernel::Agent_ABC& element );
    virtual void NotifyCreated( const kernel::Object_ABC& element );
    virtual void NotifyDeleted( const kernel::Object_ABC& element );
    virtual void NotifyCreated( const kernel::Automat_ABC& element );
    virtual void NotifyDeleted( const kernel::Automat_ABC& element );
    virtual void NotifyCreated( const kernel::ObjectKnowledge_ABC& element );
    virtual void NotifyDeleted( const kernel::ObjectKnowledge_ABC& element );
    virtual void NotifyCreated( const kernel::AgentKnowledge_ABC& element );
    virtual void NotifyDeleted( const kernel::AgentKnowledge_ABC& element );
    virtual void NotifyCreated( const kernel::PopulationKnowledge_ABC& element );
    virtual void NotifyDeleted( const kernel::PopulationKnowledge_ABC& element );

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
