// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ResolverAdapter_h_
#define __ResolverAdapter_h_

#include "Resolver_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  ResolverAdapter
    @brief  ResolverAdapter
*/
// Created: AGE 2008-07-16
// =============================================================================
template< typename BaseType, typename NewType, typename Identifier = unsigned long >
class ResolverAdapter : public Resolver_ABC< NewType, Identifier >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ResolverAdapter( const kernel::Resolver_ABC< BaseType, Identifier >& base )
        : base_( base ) {}
    virtual ~ResolverAdapter() {}
    //@}

    //! @name Operations
    //@{
    virtual NewType* Find( const Identifier& id ) const
    {
        return base_.Find( id );
    }
    virtual NewType& Get( const Identifier& id ) const
    {
        return *&base_.Get( id );
    }
    virtual Iterator< const NewType& > CreateIterator() const
    {
        return new Adapter( base_.CreateIterator() );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResolverAdapter( const ResolverAdapter& );            //!< Copy constructor
    ResolverAdapter& operator=( const ResolverAdapter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    struct Adapter : public Iterator_ABC< const NewType& >
    {
        Adapter( Iterator< const BaseType& > base ) : base_( base ) {}
        virtual bool HasMoreElements() const
        {
            return base_.HasMoreElements();
        }
        virtual const NewType& NextElement()
        {
            return *&base_.NextElement();
        }
        Iterator< const BaseType& > base_;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< BaseType, Identifier >& base_;
    //@}
};

}

#endif // __ResolverAdapter_h_
