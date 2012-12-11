// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_VIEW_H
#define WRAPPER_VIEW_H

#include <tools/Exception.h>
#include <module_api/Model.h>
#include <string>

namespace sword
{
namespace wrapper
{
// =============================================================================
/** @class  View
    @brief  View
*/
// Created: VPR 2012-01-24
// =============================================================================
class View
{
public:
    //! @name Constructors/Destructor
    //@{
    View();
    View( const SWORD_Model* model );
    //@}

    //! @name Accessors
    //@{
    operator bool() const;
    operator int8_t() const;
    operator int16_t() const;
    operator int32_t() const;
    operator int64_t() const;
    operator uint8_t() const;
    operator uint16_t() const;
    operator uint32_t() const;
    operator uint64_t() const;
    operator float() const;
    operator double() const;
    operator std::string() const;

    View operator[]( const std::string& key ) const;
    View operator[]( const char* key ) const;
    View operator[]( std::size_t key ) const;

    std::size_t GetSize() const;
    View GetElement( std::size_t index ) const;

    operator const SWORD_Model*() const;

    void* GetUserData() const;

    template< typename T >
    void VisitNamedChildren( T visitor ) const
    {
        if( ! SWORD_VisitNamedChildren( model_, &Apply< T, const char* >, AddressOf( visitor ) ) )
            throw MASA_EXCEPTION( "could not visit named children" );
    }
    template< typename T >
    void VisitIdentifiedChildren( T visitor ) const
    {
        if( ! SWORD_VisitIdentifiedChildren( model_, &Apply< T, std::size_t >, AddressOf( visitor ) ) )
            throw MASA_EXCEPTION( "could not visit identified children" );
    }
    //@}

    //! @name Operators
    //@{
    bool operator==( const View& rhs ) const;
    bool operator!=( const View& rhs ) const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T, typename K >
    static void Apply( K key, const SWORD_Model* child, void* userData )
    {
        (*static_cast< T* >( userData ))( key, child );
    }
    template< typename T >
    T* AddressOf( T& t ) const
    {
        return reinterpret_cast< T* >( &const_cast< char& >( reinterpret_cast< const char& >( t ) ) );
    }
    //@}

private:
    //! @name Member data
    //@{
    const SWORD_Model* model_;
    //@}
};

#define SWORD_MAKE_VIEW_OPERATOR( ret, op ) \
    template< typename T > \
    ret operator op( const View& view, const T& t ) \
    { \
        return static_cast< T >( view ) op t; \
    } \
    template< typename T > \
    ret operator op( const T& t, const View& view ) \
    { \
        return t op static_cast< T >( view ); \
    }

    SWORD_MAKE_VIEW_OPERATOR( T, + )
    SWORD_MAKE_VIEW_OPERATOR( T, - )
    SWORD_MAKE_VIEW_OPERATOR( T, / )
    SWORD_MAKE_VIEW_OPERATOR( T, * )
    SWORD_MAKE_VIEW_OPERATOR( bool, == )
    SWORD_MAKE_VIEW_OPERATOR( bool, != )
    SWORD_MAKE_VIEW_OPERATOR( bool, < )
    SWORD_MAKE_VIEW_OPERATOR( bool, > )
    SWORD_MAKE_VIEW_OPERATOR( bool, <= )
    SWORD_MAKE_VIEW_OPERATOR( bool, >= )

    inline std::string operator+( const char* s, const View& view )
    {
        return s + static_cast< std::string >( view );
    }
    inline std::string operator+( const View& view, const char* s )
    {
        return static_cast< std::string >( view ) + s;
    }

#undef SWORD_MAKE_VIEW_OPERATOR

}
}

#endif // WRAPPER_VIEW_H
