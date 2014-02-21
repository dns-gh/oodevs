// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Availability_h_
#define __Availability_h_

#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Displayer_ABC;
    class Entity_ABC;
    class EquipmentType;

// =============================================================================
/** @class  Availability
    @brief  Availability
*/
// Created: AGE 2006-02-28
// =============================================================================
class Availability
{
public:
    //! @name Constructors/Destructor
    //@{
             Availability();
    explicit Availability( const Availability& other );
    template< typename Message >
    Availability( const kernel::Entity_ABC& entity,
                  const tools::Resolver_ABC< kernel::EquipmentType >& resolver,
                  const Message& message )
        : entity_   ( &entity )
        , type_     ( & resolver.Get( message.equipment().id() ) )
        , total_    ( message.total() )
        , available_( message.available() )
        , atWork_   ( message.working() )
        , atRest_   ( message.has_resting() ? message.resting() : 0 )
        , lent_     ( message.has_lent() ? message.lent() : 0 )
        {};
    virtual ~Availability();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    void operator+=( const Availability& other );
    //@}

public:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* entity_;
    const kernel::EquipmentType* type_;
    unsigned int total_;
    unsigned int available_;
    unsigned int atWork_;
    unsigned int atRest_;
    unsigned int lent_;
    //@}
};
}

#endif // __Availability_h_
