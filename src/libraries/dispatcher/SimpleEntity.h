// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimpleEntity_h_
#define __SimpleEntity_h_

#include "clients_kernel/Entity_ABC.h"
#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

namespace dispatcher
{

// =============================================================================
/** @class  SimpleEntity
    @brief  SimpleEntity
*/
// Created: AGE 2008-06-20
// =============================================================================
template< typename B = kernel::Entity_ABC >
class SimpleEntity : public B
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SimpleEntity( unsigned long id, const QString& name = "" );
    virtual ~SimpleEntity();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void ApplyUpdate( const T& message )
    {
        B::Update( message );
    }

    virtual QString GetName() const;
    virtual unsigned long GetId() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleEntity( const SimpleEntity& );            //!< Copy constructor
    SimpleEntity& operator=( const SimpleEntity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    const QString name_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: SimpleEntity constructor
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
SimpleEntity< B >::SimpleEntity( unsigned long id, const QString& name /*= ""*/ )
    : id_  ( id )
    , name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleEntity destructor
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
SimpleEntity< B >::~SimpleEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleEntity::GetName
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
QString SimpleEntity< B >::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: SimpleEntity::GetId
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
unsigned long SimpleEntity< B >::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: SimpleEntity::Select
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
void SimpleEntity< B >::Select( kernel::ActionController& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleEntity::ContextMenu
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
void SimpleEntity< B >::ContextMenu( kernel::ActionController& , const QPoint& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleEntity::Activate
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename B >
void SimpleEntity< B >::Activate( kernel::ActionController& ) const
{
    // NOTHING
}

}

#endif // __SimpleEntity_h_
