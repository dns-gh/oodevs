// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Entity_ABC_h_
#define __Entity_ABC_h_

#include "tools/Extendable.h"
#include "tools/InterfaceContainer.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
    class ModelVisitor_ABC;
    class Model;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity_ABC
*/
// Created: AGE 2007-04-12
// =============================================================================
class Entity_ABC : public tools::Extendable< kernel::Extension_ABC >
                 , public tools::InterfaceContainer< kernel::Extension_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC();
    virtual ~Entity_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( ModelVisitor_ABC& visitor );
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Attach( T& extension )
    {
        tools::Extendable< kernel::Extension_ABC >::Attach( extension );
        tools::InterfaceContainer< kernel::Extension_ABC >::Register( extension );
    }

    template< typename T >
    void Update( const T& msg )
    {
        ApplyUpdate( msg );
    }

    template< typename T >
    void ApplyUpdate( const T& message )
    {
        Apply( kernel::Updatable_ABC< T >::DoUpdate, message );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity_ABC( const Entity_ABC& );            //!< Copy constructor
    Entity_ABC& operator=( const Entity_ABC& ); //!< Assignement operator
    //@}
};

}

#endif // __Entity_ABC_h_
