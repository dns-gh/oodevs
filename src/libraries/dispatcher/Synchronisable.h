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

// =============================================================================
/** @class  Synchronisable
    @brief  Synchronisable
*/
// Created: AGE 2007-04-12
// =============================================================================
class Synchronisable
{

public:
    //! @name Constructors/Destructor
    //@{
             Synchronisable();
    virtual ~Synchronisable();
    //@}

    //! @name Operations
    //@{
    void StartSynchronisation( Publisher_ABC& publisher );
    void EndSynchronisation  ( Model& model );

    virtual void SendFullUpdate   ( Publisher_ABC& publisher ) const = 0;
    virtual void CommitDestruction();
    //@}

protected:
    //! @name Operations
    //@{
    bool FlagUpdate();
    template< typename T >
    void Send( T& message );
    template< typename T >
    void SendDestruction( T& message );
    void StartSynchronisation( Synchronisable& next );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Synchronisable( const Synchronisable& );            //!< Copy constructor
    Synchronisable& operator=( const Synchronisable& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC* publisher_;
    bool updated_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Synchronisable::Send
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
template< typename T >
void Synchronisable::Send( T& message )
{
    if( ! publisher_ )
        throw std::runtime_error( __FUNCTION__ );
    message.Send( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Synchronisable::SendDestruction
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
template< typename T >
void Synchronisable::SendDestruction( T& message )
{
    Send( message );
    publisher_ = 0;
}

}

#endif // __Synchronisable_h_
