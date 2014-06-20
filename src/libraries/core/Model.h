// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef CORE_MODEL_H
#define CORE_MODEL_H

#include "Types.h"
#include "UserData.h"
#include "UserDataRetriever.h"
#include "Serialization.h"
#include "TextBuffer.h"
#include "Elements.h"
#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable: 4996 )
#endif
#include <boost/container/flat_map.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/blank.hpp>
#include <boost/ref.hpp>
#include <string>

namespace core
{
    class ModelVisitor_ABC;
    class ModelListener_ABC;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: MCO 2011-09-30
// =============================================================================
class Model : private boost::noncopyable
{
    friend void WriteModel( const Model&, std::ostream& );

public:
    //! @name Constructors/Destructor
    //@{
     Model();
    ~Model();
    //@}

    //! @name Modifiers
    //@{
    bool operator=( int8_t value );
    bool operator=( int16_t value );
    bool operator=( int32_t value );
    bool operator=( int64_t value );
    bool operator=( uint8_t value );
    bool operator=( uint16_t value );
    bool operator=( uint32_t value );
    bool operator=( uint64_t value );
    bool operator=( float value );
    bool operator=( double value );
    bool operator=( const std::string& value );

    void operator=( const Model& model );

    Model& operator[]( const std::string& key );
    Model& operator[]( const char* key );
    Model& operator[]( std::size_t key );

    void Remove();
    void Clear();

    Model& AddElement();

    template< typename T >
    void SetUserData( const T& userData )
    {
        SetData( boost::make_shared< UserData< T > >( boost::cref( userData ) ) );
    }
    void SetData( boost::shared_ptr< UserData_ABC > data );

    void Link( const Model& model );
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
    operator const std::string&() const;

    const Model& operator[]( const std::string& key ) const;
    const Model& operator[]( const char* key ) const;
    const Model& operator[]( std::size_t key ) const;

    std::size_t GetSize() const;
    Model& GetElement( std::size_t index );
    const Model& GetElement( std::size_t index ) const;
    Elements& GetElements();

    template< typename T >
    typename UserDataRetriever< T >::Result GetUserData() const
    {
        return UserDataRetriever< T >()( GetData()->Get() );
    }
    boost::shared_ptr< UserData_ABC > GetData() const;

    std::string Context() const;
    std::string Type() const;
    Model* GetParent() const;

    bool HasChild( const std::string& key ) const;
    bool HasChild( const char* key ) const;
    bool HasChild( std::size_t key ) const;
    //@}

    //! @name Operations
    //@{
    void Accept( ModelVisitor_ABC& visitor ) const;

    void Merge( Model& mergee, ModelListener_ABC& listener ) const;

    template< class Archive >
    void serialize( Archive& archive, const unsigned int /*version*/ )
    {
        archive & parent_ & content_;
    }
    //@}

public:
    //! @name Types
    //@{
    typedef boost::container::flat_map< TextBuffer, Model* > T_NamedChildren;

    typedef boost::container::flat_map< std::size_t, Model* > T_IdentifiedChildren;

    typedef boost::shared_ptr< UserData_ABC > T_Data;
    typedef const Model* T_Link;

    typedef boost::variant< boost::blank, int64_t, uint64_t, double, std::string, T_NamedChildren, T_IdentifiedChildren, Elements, T_Data, T_Link > T_Content;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit Model( Model* parent );
    //@}

    //! @name Helpers
    //@
    template< typename T > bool Assign( const T& value );
    template< typename T > T Convert() const;

    std::string FindContext( const Model& child ) const;

    template< typename T > T* AddContent();
    template< typename T > const T* GetContent() const;
    template< typename T > T* GetContent();

    Model& GetChild( const char* key, size_t len );
    const Model& GetChild( const char* key, size_t len ) const;
    Model* Find( T_NamedChildren& children, const TextBuffer& k );
    //@}

private:
    //! @name Member data
    //@{
    Model* parent_;
    T_Content content_;
    //@}
};

#define SWORD_MAKE_MODEL_OPERATOR( ret, op ) \
    template< typename T > \
    ret operator op( const Model& model, const T& t ) \
    { \
        return static_cast< T >( model ) op t; \
    } \
    template< typename T > \
    ret operator op( const T& t, const Model& model ) \
    { \
        return t op static_cast< T >( model ); \
    } \

    SWORD_MAKE_MODEL_OPERATOR( T, + )
    SWORD_MAKE_MODEL_OPERATOR( T, - )
    SWORD_MAKE_MODEL_OPERATOR( T, / )
    SWORD_MAKE_MODEL_OPERATOR( T, * )
    SWORD_MAKE_MODEL_OPERATOR( bool, == )
    SWORD_MAKE_MODEL_OPERATOR( bool, != )
    SWORD_MAKE_MODEL_OPERATOR( bool, > )
    SWORD_MAKE_MODEL_OPERATOR( bool, < )
    SWORD_MAKE_MODEL_OPERATOR( bool, >= )
    SWORD_MAKE_MODEL_OPERATOR( bool, <= )

#undef SWORD_MAKE_MODEL_OPERATOR

    inline std::string operator+( const char* c, const Model& model )
    {
        return c + static_cast< std::string >( model );
    }
    inline std::string operator+( const Model& model, const char* c )
    {
        return static_cast< std::string >( model ) + c;
    }
    inline bool operator==( const char* c, const Model& model )
    {
        return c == static_cast< std::string >( model );
    }
    inline bool operator==( const Model& model, const char* c )
    {
        return static_cast< std::string >( model ) == c;
    }
    inline bool operator!=( const char* c, const Model& model )
    {
        return c != static_cast< std::string >( model );
    }
    inline bool operator!=( const Model& model, const char* c )
    {
        return static_cast< std::string >( model ) != c;
    }
}

namespace boost
{
    template< typename Archive >
    void serialize( Archive&, blank&, const unsigned int )
    {}
}

#endif // CORE_MODEL_H
