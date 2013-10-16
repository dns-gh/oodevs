// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Resolver2_h_
#define __Resolver2_h_

#include <tools/Resolver_ABC.h>
#include <tools/Exception.h>
#include <tools/Iterator.h>
#include <stdexcept>
#include <sstream>

namespace kernel
{

// =============================================================================
/** @class  Resolver2
    @brief  Resolver2 base class
*/
// Created: AGE 2006-02-10
// =============================================================================
template< typename T, typename Identifier1 = unsigned long, typename Identifier2 = std::string >
class Resolver2 : public  tools::Resolver_ABC< T, Identifier1 >
                , public  tools::Resolver_ABC< T, Identifier2 >
{
public:
    //! @name Constructors/Destructor
    //@{
             Resolver2() {};
    virtual ~Resolver2() {};
    //@}

    //! @name Operations
    //@{
    template< typename Functor >
    void Apply( const Functor& functor )
    {
        for( IT_Elements1 it = elements1_.begin(); it != elements1_.end(); ++it )
            functor( *it->second );
    };

    template< typename Functor >
    void Apply( const Functor& functor ) const
    {
        for( auto it = elements1_.begin(); it != elements1_.end(); ++it )
        {
            const T& element = *it->second;
            functor( element );
        }
    };

    virtual tools::Iterator< const T& > CreateIterator() const
    {
        return new tools::AssociativeIterator< const T&, T_Elements1 >( elements1_ );
    };

    void Register( const Identifier1& identifier1, const Identifier2& identifier2, T& element )
    {
        T*& p1 = elements1_[ identifier1 ];
        T*& p2 = elements2_[ identifier2 ];
        if( p1 || p2 )
            Error( identifier1, "already registered" );
        p1 = p2 = &element;
    }
    void Remove( const Identifier1& identifier )
    {
        T* element = Find( identifier );
        for( IT_Elements2 it = elements2_.begin(); it != elements2_.end(); ++it )
            if( it->second == element )
            {
                elements2_.erase( it );
                break;
            }
        elements1_.erase( identifier );
    }

    void Remove( const Identifier2& identifier )
    {
        T* element = Find( identifier );
        for( IT_Elements1 it = elements1_.begin(); it != elements1_.end(); ++it )
            if( it->second == element )
            {
                elements1_.erase( it );
                break;
            }
        elements2_.erase( identifier );
    }

    void DeleteAll()
    {
        while( ! elements1_.empty() )
        {
            IT_Elements1 it = elements1_.begin();
            const T* element = it->second;
            Remove( it->first );
            delete element;
        }
    }

    void Clear()
    {
        elements1_.clear();
        elements2_.clear();
    }

    virtual T* Find( const Identifier1& identifier ) const
    {
        CIT_Elements1 it = elements1_.find( identifier );
        if( it != elements1_.end() )
            return it->second;
        return 0;
    }
    virtual T* Find( const Identifier2& identifier ) const
    {
        CIT_Elements2 it = elements2_.find( identifier );
        if( it != elements2_.end() )
            return it->second;
        return 0;
    }
    virtual T& Get ( const Identifier1& identifier ) const
    {
        T* element = Find( identifier );
        if( ! element )
            Error( identifier, "does not exist" );
        return *element;
    }
    virtual T& Get ( const Identifier2& identifier ) const
    {
        T* element = Find( identifier );
        if( ! element )
            Error( identifier, "does not exist" );
        return *element;
    }
    unsigned long Count() const
    {
        return static_cast< unsigned long >( elements1_.size() );
    }
    //@}
    // $$$$ AGE 2006-02-14:
protected:
    //! @name Copy/Assignment
    //@{
    Resolver2( const Resolver2& );            //!< Copy constructor
    Resolver2& operator=( const Resolver2& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Error( const Identifier1& identifier, const std::string& message ) const
    {
        std::stringstream str;
        str << "Element type '" << typeid( T ).name() << "' '" << identifier << "' " << message;
        throw MASA_EXCEPTION( str.str() );
    }
    void Error( const Identifier2& identifier, const std::string& message ) const
    {
        std::stringstream str;
        str << "Element type '" << typeid( T ).name() << "' '" << identifier << "' " << message;
        throw MASA_EXCEPTION( str.str() );
    }
    //@}

    //! @name Types
    //@{
    typedef std::map< Identifier1, T* >             T_Elements1;
    typedef typename T_Elements1::iterator         IT_Elements1;
    typedef typename T_Elements1::const_iterator  CIT_Elements1;

    typedef std::map< Identifier2, T* >             T_Elements2;
    typedef typename T_Elements2::iterator         IT_Elements2;
    typedef typename T_Elements2::const_iterator  CIT_Elements2;
    //@}

protected:
    //! @name Member data
    //@{
    T_Elements1 elements1_;
    T_Elements2 elements2_;
    //@}
};

}

#endif // __Resolver2_h_
