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

#include "reports/RcEntityResolver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Agent_ABC;
    class KnowledgeConverter_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Object_ABC;
    class Population_ABC;
    class UrbanObject_ABC;
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
                       , public tools::Resolver< kernel::Object_ABC >
                       , public tools::Resolver< kernel::UrbanObject_ABC >
                       , public tools::Resolver< kernel::Population_ABC >
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                       , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                       , public tools::ElementObserver_ABC< kernel::Object_ABC >
                       , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
                       , public tools::ElementObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             RcEntityResolver( kernel::Controllers& controllers, const kernel::KnowledgeConverter_ABC& converter );
    virtual ~RcEntityResolver();
    //@}

private:
    //! @name Operations
    //@{
    virtual QString CreateLink( const kernel::Entity_ABC* entity, const std::string& type, unsigned long id ) const;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& element );
    virtual void NotifyDeleted( const kernel::Agent_ABC& element );
    virtual void NotifyCreated( const kernel::Object_ABC& element );
    virtual void NotifyDeleted( const kernel::Object_ABC& element );
    virtual void NotifyCreated( const kernel::Automat_ABC& element );
    virtual void NotifyDeleted( const kernel::Automat_ABC& element );
    virtual void NotifyCreated( const kernel::Population_ABC& element );
    virtual void NotifyDeleted( const kernel::Population_ABC& element );
    virtual void NotifyCreated( const kernel::UrbanObject_ABC& element );
    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& element );

    template< typename T >
    QString CreateLink( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::KnowledgeConverter_ABC& converter_;
    //@}
};

#endif // __RcEntityResolver_h_
