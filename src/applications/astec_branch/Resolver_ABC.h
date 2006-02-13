// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Resolver_ABC_h_
#define __Resolver_ABC_h_

#include <stdexcept>

// =============================================================================
/** @class  Resolver_ABC
    @brief  Resolver base class
*/
// Created: AGE 2006-02-10
// =============================================================================
template< typename T >
class Resolver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Resolver_ABC() {};
    virtual ~Resolver_ABC() {};
    //@}

    //! @name Operations
    //@{
    template< typename Functor >
    void Apply( const Functor& functor ) 
    {
        for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            functor( *it->second );
    };

    void Register( unsigned long identifier, T& element )
    {
        T*& p = elements_[ identifier ];
        if( p )
            throw std::runtime_error( std::string( "Element type '" ) + typeid( T ).name() + "' already registered" );
        p = &element;
    }
    void Remove( unsigned long identifier )
    {
        elements_.erase( identifier );
    }

    void DeleteAll() 
    {
        for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            delete it->second;
        elements_.clear();
    };


    T* Find( unsigned long identifier ) const
    {
        CIT_Elements it = elements_.find( identifier );
        if( it != elements_.end() )
            return it->second;
        return 0;
    }
    T& Get ( unsigned long identifier ) const
    {
        T* element = Find( identifier );
        if( ! element )
            throw std::runtime_error( std::string( "Element type '" ) + typeid( T ).name() + "' does not exist" );
        return *element;
    }
    //@}

protected: // $$$$ AGE 2006-02-13: 
    //! @name Copy/Assignement
    //@{
    Resolver_ABC( const Resolver_ABC& );            //!< Copy constructor
    Resolver_ABC& operator=( const Resolver_ABC& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, T* >          T_Elements;
    typedef typename T_Elements::iterator         IT_Elements;
    typedef typename T_Elements::const_iterator  CIT_Elements;
    //@}

protected:
    //! @name Member data
    //@{
    T_Elements elements_;
    //@}
};

#endif // __Resolver_ABC_h_
