// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Synchronisable_h_
#define __Synchronisable_h_

#include "Model.h"

namespace dispatcher
{
    class Publisher_ABC;
    class ModelVisitor_ABC;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity_ABC
*/
// Created: AGE 2007-04-12
// =============================================================================
class Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC();
    virtual ~Entity_ABC();
    //@}

    //! @name Operations
    //@{
    void StartSynchronisation( Publisher_ABC& publisher, bool create );
    void EndSynchronisation  ( Model& model );

    virtual void SendFullUpdate( Publisher_ABC& publisher ) const = 0;
    virtual void SendCreation  ( Publisher_ABC& publisher ) const = 0;
    virtual void CommitDestruction();
    //@}

protected:
    //! @name Operations
    //@{
    bool FlagUpdate();
    template< typename T >
    void Send( T& message );
    void StartSynchronisation( Entity_ABC& next, bool create );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity_ABC( const Entity_ABC& );            //!< Copy constructor
    Entity_ABC& operator=( const Entity_ABC& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC* publisher_;
    bool updated_;
    bool create_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Send
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
template< typename T >
void Entity_ABC::Send( T& message )
{
    if( ! publisher_ )
        throw std::runtime_error( typeid( *this ).name() + std::string( "::Send" ) );
    message.Send( *publisher_ );
}

}

#endif // __Synchronisable_h_
