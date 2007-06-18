// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Resolver_h_
#define __Resolver_h_

#include "Resolver_ABC.h"
#include "Iterator.h"
#include <stdexcept>
#include <sstream>

namespace kernel
{

// =============================================================================
/** @class  Resolver
    @brief  Resolver base class
*/
// Created: AGE 2006-02-10
// =============================================================================
template< typename T, typename Identifier = unsigned long >
class Resolver : public Resolver_ABC< T, Identifier >
{

public:
    //! @name Constructors/Destructor
    //@{
             Resolver() {};
    virtual ~Resolver() {};
    //@}

    //! @name Operations
    //@{
    template< typename Functor >
    void Apply( const Functor& functor ) 
    {
        for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            functor( *it->second );
    };

    template< typename Functor >
    void Apply( const Functor& functor ) const
    {
        for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        {
            const T& element = *it->second;
            functor( element );
        }
    };

    virtual Iterator< const T& > CreateIterator() const
    {
        return new AssociativeIterator< const T&, T_Elements >( elements_ );
    };

    void Register( const Identifier& identifier, T& element )
    {
        T*& p = elements_[ identifier ];
        if( p )
            Error( identifier, "already registered" );
        p = &element;
    }
    void Remove( const Identifier& identifier )
    {
        elements_.erase( identifier );
    }

    void DeleteAll() 
    {
        while( ! elements_.empty() )
        {
            IT_Elements it = elements_.begin();
            const T* element = it->second;
            elements_.erase( it );
            delete element;
        }
    }

    void Clear()
    {
        elements_.clear();
    }

    virtual T* Find( const Identifier& identifier ) const
    {
        CIT_Elements it = elements_.find( identifier );
        if( it != elements_.end() )
            return it->second;
        return 0;
    }
    virtual T& Get ( const Identifier& identifier ) const
    {
        T* element = Find( identifier );
        if( ! element )
            Error( identifier, "does not exist" );
        return *element;
    }
    unsigned long Count() const
    {
        return elements_.size();
    }
    //@}
    
private:
    //! @name Copy/Assignement
    //@{
    Resolver( const Resolver& );            //!< Copy constructor
    Resolver& operator=( const Resolver& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    void Error( const Identifier& identifier, const std::string& message ) const
    {
        std::stringstream str;
        str << "Element type '" << typeid( T ).name() << "' '" << identifier << "' " << message;
        throw std::runtime_error( str.str() );
    }
    //@}

    //! @name Types
    //@{
    typedef std::map< Identifier, T* >             T_Elements;
    typedef typename T_Elements::iterator         IT_Elements;
    typedef typename T_Elements::const_iterator  CIT_Elements;
    //@}

protected:
    //! @name Member data
    //@{
    T_Elements elements_;
    //@}
};

template< typename T >
class StringResolver : public Resolver< T, QString >
{
public:
             StringResolver() : Resolver< T, QString >() {}
    virtual ~StringResolver() {}
};

}

#endif // __Resolver_h_
