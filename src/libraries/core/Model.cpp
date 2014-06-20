// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Model.h"
#include "ModelVisitor_ABC.h"
#include "ModelListener_ABC.h"
#include "Exceptions.h"
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4702 )
#endif
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/bind.hpp>
#include <boost/swap.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#   pragma warning( disable: 4503 )
#endif

using namespace core;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: SLI 2011-12-16
// -----------------------------------------------------------------------------
Model::Model()
    : parent_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: MCO 2012-01-19
// -----------------------------------------------------------------------------
Model::Model( Model* parent )
    : parent_( parent )
{
    // NOTHING
}

namespace
{
    struct PurgeVisitor : boost::static_visitor<>
    {
        void operator()( Model::T_NamedChildren& children ) const
        {
            for( Model::T_NamedChildren::iterator it = children.begin(); it != children.end(); ++it )
                delete it->second;
            children.clear();
        }
        void operator()( Model::T_IdentifiedChildren& children ) const
        {
            for( Model::T_IdentifiedChildren::iterator it = children.begin(); it != children.end(); ++it )
                delete it->second;
            children.clear();
        }
        void operator()( Elements& elements ) const
        {
            elements.Clear();
        }
        template< typename T >
        void operator()( T& ) const
        {}
    };
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: MCO 2012-01-19
// -----------------------------------------------------------------------------
Model::~Model()
{
    boost::apply_visitor( PurgeVisitor(), content_ );
}

namespace
{
    struct ClearVisitor : boost::static_visitor<>
    {
        void operator()( Model::T_Link, const Model& model ) const
        {
            throw RuntimeError( "cannot clear link node " + model.Context() );
        }
        template< typename T >
        void operator()( T& t, const Model& ) const
        {
            PurgeVisitor()( t );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Clear
// Created: SLI 2012-09-19
// -----------------------------------------------------------------------------
void Model::Clear()
{
    boost::apply_visitor( boost::bind( ClearVisitor(), _1, boost::cref( *this ) ), content_ );
    content_ = boost::blank();
}

namespace
{
    template< typename T >
    struct ConvertVisitor : boost::static_visitor< T >
    {
        template< typename U >
        T operator()( const U& value, const Model&,
            typename boost::enable_if< boost::is_arithmetic< U > >::type* = 0 ) const
        {
            return boost::numeric_cast< T >( value );
        }
        template< typename U >
        T operator()( const U&, const Model& model,
            typename boost::disable_if< boost::is_arithmetic< U > >::type* = 0 ) const
        {
            throw RuntimeError( "invalid numeric conversion for " + model.Context() + " as it contains " + model.Type() );
        }
        T operator()( Model::T_Link link, const Model& ) const
        {
            return *link;
        }
    };
    template<>
    struct ConvertVisitor< bool > : boost::static_visitor< bool >
    {
        template< typename U >
        bool operator()( const U& value, const Model&,
            typename boost::enable_if< boost::is_arithmetic< U > >::type* = 0 ) const
        {
            return !!value;
        }
        template< typename U >
        bool operator()( const U&, const Model& model,
            typename boost::disable_if< boost::is_arithmetic< U > >::type* = 0 ) const
        {
            throw RuntimeError( "invalid boolean conversion for " + model.Context() + " as it contains " + model.Type() );
        }
        bool operator()( Model::T_Link link, const Model& ) const
        {
            return *link;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Convert
// Created: MCO 2012-03-05
// -----------------------------------------------------------------------------
template< typename T >
T Model::Convert() const
{
    return apply_visitor( boost::bind( ConvertVisitor< T >(), _1, boost::cref( *this ) ), content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::operator <type>
// Created: BAX 2012-01-25
// -----------------------------------------------------------------------------
Model::operator bool()     const { return Convert< bool     >(); }
Model::operator int8_t()   const { return Convert< int8_t   >(); }
Model::operator int16_t()  const { return Convert< int16_t  >(); }
Model::operator int32_t()  const { return Convert< int32_t  >(); }
Model::operator int64_t()  const { return Convert< int64_t  >(); }
Model::operator uint8_t()  const { return Convert< uint8_t  >(); }
Model::operator uint16_t() const { return Convert< uint16_t >(); }
Model::operator uint32_t() const { return Convert< uint32_t >(); }
Model::operator uint64_t() const { return Convert< uint64_t >(); }
Model::operator float()    const { return Convert< float    >(); }
Model::operator double()   const { return Convert< double   >(); }

// -----------------------------------------------------------------------------
// Name: Model::operator const std::string&
// Created: BAX 2012-01-25
// -----------------------------------------------------------------------------
Model::operator const std::string&() const
{
    const std::string* s = GetContent< std::string >();
    if( ! s )
        throw RuntimeError( "cannot access content of " + Context() + " as string because it contains " + Type() );
    return *s;
}

// -----------------------------------------------------------------------------
// Name: Model::AddContent
// Created: MCO 2012-06-04
// -----------------------------------------------------------------------------
template< typename T >
T* Model::AddContent()
{
    static const int index = 0;
    BOOST_MPL_ASSERT(( boost::is_same< boost::blank, boost::mpl::at_c< T_Content::types, index >::type > ));
    if( content_.which() == index )
        content_ = T();
    return GetContent< T >();
}

// -----------------------------------------------------------------------------
// Name: Model::GetContent
// Created: MCO 2012-12-10
// -----------------------------------------------------------------------------
template< typename T >
const T* Model::GetContent() const
{
    const T* content = boost::get< T >( &content_ );
    if( content )
        return content;
    const T_Link* link = boost::get< const T_Link >( &content_ );
    if( ! link )
        return 0;
    return (*link)->GetContent< T >();
}

// -----------------------------------------------------------------------------
// Name: T* Model::GetContent
// Created: MCO 2012-12-10
// -----------------------------------------------------------------------------
template< typename T > T* Model::GetContent()
{
    return boost::get< T >( &content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::Link
// Created: MCO 2012-12-10
// -----------------------------------------------------------------------------
void Model::Link( const Model& model )
{
    T_Link* link = AddContent< T_Link >();
    if( ! link )
        throw RuntimeError( "cannot add link in " + Context() + " as it contains " + Type() );
    (*link) = &model;
}

namespace
{
    template< typename T >
    struct is_value :
        boost::mpl::or_<
            boost::is_arithmetic< T >,
            boost::is_same< T, std::string >
        >
    {};
    template< typename T1, typename T2 >
    struct are_values :
        boost::mpl::and_<
            is_value< T1 >,
            is_value< T2 >
        >
    {};

    struct AssignValueVisitor : boost::static_visitor< bool >
    {
        template< typename T, typename Content >
        bool operator()( const T& value, boost::blank, const Model&, Content& content,
            typename boost::enable_if< is_value< T > >::type* = 0 ) const
        {
            content = value;
            return true;
        }
        template< typename T, typename Content >
        bool operator()( const T& value, T rhs, const Model&, Content& content,
            typename boost::enable_if< is_value< T > >::type* = 0 ) const
        {
            content = value;
            return value != rhs;
        }
        template< typename T1, typename T2, typename Content >
        bool operator()( const T1& value, const T2&, const Model&, Content& content,
            typename boost::enable_if< are_values< T1, T2 > >::type* = 0 ) const
        {
            content = value;
            return true;
        }
        template< typename T1, typename T2, typename Content >
        bool operator()( const T1&, const T2&, const Model& model, Content&,
            typename boost::disable_if< are_values< T1, T2 > >::type* = 0 ) const
        {
            throw RuntimeError( "cannot assign value " + model.Context() + " as it contains " + model.Type() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Assign
// Created: MCO 2012-02-29
// -----------------------------------------------------------------------------
template< typename T >
bool Model::Assign( const T& value )
{
    return apply_visitor( boost::bind( AssignValueVisitor(), value, _1, boost::cref( *this ), boost::ref( content_ ) ), content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::operator=
// Created: BAX 2012-01-25
// -----------------------------------------------------------------------------
bool Model::operator=( int8_t             value ) { return Assign< int64_t >( value ); }
bool Model::operator=( int16_t            value ) { return Assign< int64_t >( value ); }
bool Model::operator=( int32_t            value ) { return Assign< int64_t >( value ); }
bool Model::operator=( int64_t            value ) { return Assign( value ); }
bool Model::operator=( uint8_t            value ) { return Assign< uint64_t >( value ); }
bool Model::operator=( uint16_t           value ) { return Assign< uint64_t >( value ); }
bool Model::operator=( uint32_t           value ) { return Assign< uint64_t >( value ); }
bool Model::operator=( uint64_t           value ) { return Assign( value ); }
bool Model::operator=( float              value ) { return Assign< double >( value ); }
bool Model::operator=( double             value ) { return Assign( value ); }
bool Model::operator=( const std::string& value ) { return Assign( value ); }

namespace
{
    struct CopyVisitor : boost::static_visitor<>
    {
        void operator()( Model& model, const Model::T_NamedChildren& children ) const
        {
            for( Model::T_NamedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                model[ it->first.String() ] = *it->second;
        }
        void operator()( Model& model, const Model::T_IdentifiedChildren& children ) const
        {
            for( Model::T_IdentifiedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                model[ it->first ] = *it->second;
        }
        void operator()( Model& model, const Elements& elements ) const
        {
            elements.Copy( model );
        }
        void operator()( Model& model, const Model::T_Data& data ) const
        {
            model.SetData( data );
        }
        void operator()( Model& model, Model::T_Link link ) const
        {
            model.Link( *link );
        }
        void operator()( Model& /*model*/, boost::blank ) const
        {
            // NOTHING
        }
        template< typename T >
        void operator()( Model& model, const T& t ) const
        {
            model = t;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::operator=
// Created: MCO 2012-08-23
// -----------------------------------------------------------------------------
void Model::operator=( const Model& model )
{
    if( content_.which() != 0 && content_.which() != model.content_.which() )
        throw RuntimeError( "cannot copy " + model.Context() + " into " + Context() + " as it contains " + model.Type() + " and destination contains " + Type() );
    boost::apply_visitor( PurgeVisitor(), content_ );
    boost::apply_visitor( boost::bind( CopyVisitor(), boost::ref( *this ), _1 ), model.content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::AddElement
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
Model& Model::AddElement()
{
    Elements* elements = AddContent< Elements >();
    if( ! elements )
        throw RuntimeError( "cannot add elements in " + Context() + " as it contains " + Type() );
    Model* child = new Model( this );
    return elements->Add( child );
}

// -----------------------------------------------------------------------------
// Name: Model::GetElement
// Created: MCO 2012-06-13
// -----------------------------------------------------------------------------
Model& Model::GetElement( std::size_t index )
{
    Elements* elements = GetContent< Elements >();
    if( ! elements )
        throw RuntimeError( "cannot get element from " + Context() + " as it contains " + Type() );
    if( elements->Size() <= index )
        throw RuntimeError( "cannot get element '" + boost::lexical_cast< std::string >( index )
            + "' from " + Context() + " as its current size is " + boost::lexical_cast< std::string >( elements->Size() ) );
    return (*elements)[ index ];
}

// -----------------------------------------------------------------------------
// Name: Model::GetElement
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
const Model& Model::GetElement( std::size_t index ) const
{
    const Elements* elements = GetContent< Elements >();
    if( ! elements )
        throw RuntimeError( "cannot get element from " + Context() + " as it contains " + Type() );
    if( elements->Size() <= index )
        throw RuntimeError( "cannot get element '" + boost::lexical_cast< std::string >( index )
            + "' from " + Context() + " as its current size is " + boost::lexical_cast< std::string >( elements->Size() ) );
    return (*elements)[ index ];
}

// -----------------------------------------------------------------------------
// Name: Model::GetElements
// Created: BAX 2012-10-18
// -----------------------------------------------------------------------------
Elements& Model::GetElements()
{
    Elements* elements = AddContent< Elements >();
    if( ! elements )
        throw RuntimeError( "cannot get elements from " + Context() + " as it contains " + Type() );
    return *elements;
}

// -----------------------------------------------------------------------------
// Name: Model::operator[]
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
Model& Model::operator[]( const std::string& key )
{
    return GetChild( key.c_str(), key.size() );
}

// -----------------------------------------------------------------------------
// Name: Model::operator[]
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
const Model& Model::operator[]( const std::string& key ) const
{
    return GetChild( key.c_str(), key.size() );
}

// -----------------------------------------------------------------------------
// Name: Model::operator[]
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
const Model& Model::operator[]( const char* key ) const
{
    return GetChild( key, strlen( key ) );
}

// -----------------------------------------------------------------------------
// Name: Model::operator[]
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
Model& Model::operator[]( const char* key )
{
    return GetChild( key, strlen( key ) );
}

namespace
{
    std::string SubContext( const Model& model, const char* begin, const char* end )
    {
        const std::string root = model.Context();
        if( end <= begin )
            return root;
        const std::string suffix( begin, end - begin );
        if( root.size() == 1 )
            return root + suffix;
        return root + "/" + suffix;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::GetChild
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
Model& Model::GetChild( const char* key, size_t len )
{
    if( ! key )
        throw RuntimeError( "cannot insert null key in " + Context() );
    if( len == 0 )
        return *this;
    T_NamedChildren* children = AddContent< T_NamedChildren >();
    if( ! children )
        throw RuntimeError( "cannot insert '" + std::string( key, len ) + "' in " + Context() + " as it contains " + Type() );
    const char* pos = std::find( key, key + len, '/' );
    Model* child = Find( *children, TextBuffer( key, pos - key ) );
    if( pos == key + len )
        return *child;
    return child->GetChild( pos + 1, len + key - pos - 1 );
}

// -----------------------------------------------------------------------------
// Name: Model::GetChild
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
const Model& Model::GetChild( const char* key, size_t len ) const
{
    if( ! key )
        throw RuntimeError( "cannot find null key in " + Context() );
    if( len == 0 )
        return *this;
    const T_NamedChildren* children = GetContent< T_NamedChildren >();
    if( ! children )
        throw RuntimeError( "cannot find '" + std::string( key, len ) + "' in " + Context() + " as it contains " + Type() );
    const char* pos = std::find( key, key + len, '/' );
    T_NamedChildren::const_iterator it = children->find( TextBuffer( key, pos - key ) );
    if( it == children->end() )
        throw RuntimeError( "cannot find '" + std::string( key, pos - key ) + "' in " + Context() );
    const Model& child = *it->second;
    if( pos == key + len )
        return child;
    return child.GetChild( pos + 1, len + key - pos - 1 );
}

// -----------------------------------------------------------------------------
// Name: Model::operator[]
// Created: VPR 2012-01-17
// -----------------------------------------------------------------------------
Model& Model::operator[]( std::size_t key )
{
    T_IdentifiedChildren* children = AddContent< T_IdentifiedChildren >();
    if( ! children )
        throw RuntimeError( "cannot insert '" + boost::lexical_cast< std::string >( key ) + "' in " + Context() + " as it contains " + Type() );
    Model*& child = (*children)[ key ];
    if( ! child )
        child = new Model( this );
    return *child;
}

// -----------------------------------------------------------------------------
// Name: Model::operator[]
// Created: VPR 2012-01-17
// -----------------------------------------------------------------------------
const Model& Model::operator[]( std::size_t key ) const
{
    const T_IdentifiedChildren* children = GetContent< T_IdentifiedChildren >();
    if( ! children )
        throw RuntimeError( "cannot find '" + boost::lexical_cast< std::string >( key ) + "' in " + Context() + " as it contains " + Type() );
    T_IdentifiedChildren::const_iterator it = children->find( key );
    if( it == children->end() )
        throw RuntimeError( "cannot find '" + boost::lexical_cast< std::string >( key ) + "' in " + Context() );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: Model::Find
// Created: MCO 2012-10-11
// -----------------------------------------------------------------------------
Model* Model::Find( T_NamedChildren& children, const TextBuffer& key )
{
    std::pair< T_NamedChildren::iterator, bool > result =
        children.insert( std::make_pair( key, static_cast< Model* >( 0 ) ) );
    Model*& child = result.first->second;
    if( result.second )
    {
        result.first->first.Store();
        child = new Model( this );
    }
    return child;
}

namespace
{
    struct RemoveVisitor : boost::static_visitor<>
    {
        void operator()( Model::T_NamedChildren& children, const Model& child ) const
        {
            for( Model::T_NamedChildren::iterator it = children.begin(); it != children.end(); ++it )
                if( it->second == &child )
                {
                    children.erase( it );
                    delete &child;
                    return;
                }
        }
        void operator()( Model::T_IdentifiedChildren& children, const Model& child ) const
        {
            for( Model::T_IdentifiedChildren::iterator it = children.begin(); it != children.end(); ++it )
                if( it->second == &child )
                {
                    children.erase( it );
                    delete &child;
                    return;
                }
        }
        void operator()( Elements& elements, const Model& child ) const
        {
            elements.Remove( child );
        }
        template< typename T >
        void operator()( const T&, const Model& ) const
        {
            // NOTHING
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Remove
// Created: MCO 2012-09-06
// -----------------------------------------------------------------------------
void Model::Remove()
{
    if( ! parent_ )
        throw RuntimeError( "cannot remove root node" );
    boost::apply_visitor( boost::bind( RemoveVisitor(), _1, boost::ref( *this ) ), parent_->content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::GetSize
// Created: SLI 2011-12-09
// -----------------------------------------------------------------------------
std::size_t Model::GetSize() const
{
    const Elements* elements = GetContent< Elements >();
    if( elements )
        return elements->Size();
    return 0;
}

namespace
{
    struct ModelVisitor : boost::static_visitor<>
    {
        void operator()( const Model::T_NamedChildren& children, ModelVisitor_ABC& visitor ) const
        {
            for( Model::T_NamedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                visitor.Visit( it->first.String(), *it->second );
        }
        void operator()( const Model::T_IdentifiedChildren& children, ModelVisitor_ABC& visitor ) const
        {
            for( Model::T_IdentifiedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                visitor.Visit( it->first, *it->second );
        }
        void operator()( const Elements& elements, ModelVisitor_ABC& visitor ) const
        {
            for( std::size_t i = 0; i < elements.Size(); ++i )
                visitor.Visit( elements[ i ] );
        }
        void operator()( const Model::T_Data& data, ModelVisitor_ABC& visitor ) const
        {
            if( data )
            {
                assert( data->Get() );
                visitor.Visit( data );
            }
        }
        void operator()( Model::T_Link link, ModelVisitor_ABC& visitor ) const
        {
            link->Accept( visitor );
        }
        void operator()( boost::blank, ModelVisitor_ABC& ) const
        {
            // NOTHING
        }
        template< typename T >
        void operator()( const T& value, ModelVisitor_ABC& visitor ) const
        {
            visitor.Visit( value );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Accept
// Created: MCO 2012-04-06
// -----------------------------------------------------------------------------
void Model::Accept( ModelVisitor_ABC& visitor ) const
{
    boost::apply_visitor( boost::bind( ModelVisitor(), _1, boost::ref( visitor ) ), content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::SetData
// Created: SLI 2012-01-02
// -----------------------------------------------------------------------------
void Model::SetData( Model::T_Data data )
{
    T_Data* d = AddContent< T_Data >();
    if( ! d )
        throw RuntimeError( "cannot set user data in " + Context() + " as it contains " + Type() );
    if( ! data )
        throw RuntimeError( "cannot set empty user data in " + Context() );
    *d = data;
}

// -----------------------------------------------------------------------------
// Name: Model::GetData
// Created: VPR 2011-12-22
// -----------------------------------------------------------------------------
Model::T_Data Model::GetData() const
{
    const T_Data* data = GetContent< T_Data >();
    if( ! data )
        throw RuntimeError( "cannot find user data in " + Context() );
    return *data;
}

// -----------------------------------------------------------------------------
// Name: Model::Context
// Created: MCO 2012-01-19
// -----------------------------------------------------------------------------
std::string Model::Context() const
{
    if( parent_ )
        return parent_->FindContext( *this );
    return "/";
}

namespace
{
    struct NameVisitor : boost::static_visitor< std::string >
    {
        std::string operator()( const Model::T_NamedChildren& children, const Model& child ) const
        {
            for( Model::T_NamedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                if( it->second == &child )
                    return it->first.String();
            return "**child_not_found**";
        }
        std::string operator()( const Model::T_IdentifiedChildren& children, const Model& child ) const
        {
            for( Model::T_IdentifiedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                if( it->second == &child )
                    return boost::lexical_cast< std::string >( it->first );
            return "**child_not_found**";
        }
        std::string operator()( const Elements& elements, const Model& child ) const
        {
            const std::size_t idx = elements.Index( child );
            if( idx == SIZE_MAX )
                return "**element_not_found**";
            return boost::lexical_cast< std::string >( idx );
        }
        template< typename T >
        std::string operator()( const T&, const Model& ) const
        {
            return "**error**";
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::FindContext
// Created: MCO 2012-06-01
// -----------------------------------------------------------------------------
std::string Model::FindContext( const Model& child ) const
{
    return ( parent_ ? parent_->FindContext( *this ) : "" ) + "/"
        + boost::apply_visitor( boost::bind( NameVisitor(), _1, boost::cref( child ) ), content_ );
}

// -----------------------------------------------------------------------------
// Name: Model::Type
// Created: MCO 2012-06-05
// -----------------------------------------------------------------------------
std::string Model::Type() const
{
    static const char* types[] = { "nothing", "value", "value", "value", "value", "string children", "integer children", "elements", "user data", "link" };
    BOOST_STATIC_ASSERT( sizeof( types ) / sizeof( char* ) == boost::mpl::size< T_Content::types >::value );
    return types[ content_.which() ];
}

// -----------------------------------------------------------------------------
// Name: Model::GetParent
// Created: BAX 2012-10-15
// -----------------------------------------------------------------------------
Model* Model::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Model::HasChild
// Created: ABR 2013-07-31
// -----------------------------------------------------------------------------
bool Model::HasChild( const std::string& key ) const
{
    return HasChild( key.c_str() );
}

// -----------------------------------------------------------------------------
// Name: Model::HasChild
// Created: ABR 2013-07-31
// -----------------------------------------------------------------------------
bool Model::HasChild( const char* key ) const
{
    std::size_t len = strlen( key );
    if( ! key )
        return false;
    if( ! len )
        return false;
    const Model* current = this;
    const char* ptr = key;
    if( ptr[ len - 1 ] == '/' )
        --len;
    while( len )
    {
        const T_NamedChildren* children = boost::get< T_NamedChildren >( &current->content_ );
        if( ! children )
            return false;
        const char* pos = std::find( ptr, ptr + len, '/' );
        T_NamedChildren::const_iterator it = children->find( TextBuffer( ptr, pos - ptr ) );
        if( it == children->end() )
            return false;
        current = it->second;
        if( pos == ptr + len )
            return true;
        len -= pos + 1 - ptr;
        ptr = pos + 1;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Model::HasChild
// Created: ABR 2013-07-31
// -----------------------------------------------------------------------------
bool Model::HasChild( std::size_t key ) const
{
    const T_IdentifiedChildren* children = boost::get< T_IdentifiedChildren >( &content_ );
    return children && children->find( key ) != children->end();
}

namespace
{
    struct MergeVisitor : boost::static_visitor<>
    {
        template< typename T >
        void operator()( boost::blank, const T&, Model&, ModelListener_ABC& ) const
        {
            // NOTHING
        }
        void operator()( boost::blank, const Elements& elements, Model& mergee, ModelListener_ABC& listener ) const
        {
            const Model::T_Content mergerElements = Elements();
            const Model::T_Content mergeeElements = elements;
            apply_visitor( boost::bind( MergeVisitor(), _1, _2, boost::ref( mergee ), boost::ref( listener ) ), mergerElements, mergeeElements );
        }
        template< typename T, typename U >
        void operator()( T value, const U&, Model& mergee, ModelListener_ABC& listener ) const
        {
            if( mergee = value )
                listener.NotifyChanged( mergee );
        }
        template< typename T >
        void operator()( const Model::T_Data& data, const T&, Model& mergee, ModelListener_ABC& ) const
        {
            mergee.SetData( data );
        }
        template< typename T >
        void operator()( const Model::T_NamedChildren& children, const T&, Model& mergee, ModelListener_ABC& listener ) const
        {
            for( Model::T_NamedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                it->second->Merge( mergee[ it->first.String() ], listener );
        }
        template< typename T >
        void operator()( const Model::T_IdentifiedChildren& children, const T&, Model& mergee, ModelListener_ABC& listener ) const
        {
            for( Model::T_IdentifiedChildren::const_iterator it = children.begin(); it != children.end(); ++it )
                it->second->Merge( mergee[ it->first ], listener );
        }
        template< typename T >
        void operator()( Model::T_Link, const T&, Model& mergee, ModelListener_ABC& ) const
        {
            throw RuntimeError( "merging link from " + mergee.Context() + " is not allowed" );
        }
        template< typename T >
        void operator()( const Elements& src, const T&, Model& mergee, ModelListener_ABC& listener ) const
        {
            Elements& dst = mergee.GetElements();
            const std::size_t common = std::min( dst.Size(), src.Size() );
            for( std::size_t i = 0; i < common; ++i )
                src[ i ].Merge( dst[ i ], listener );
            const bool modified = dst.Size() != src.Size();
            const std::size_t destination = dst.Size();
            for( std::size_t i = common; i < destination; ++i )
            {
                const Model& next = dst.Hide( dst.Size() - 1 );
                listener.NotifyRemoved( next );
            }
            for( std::size_t i = common; i < src.Size(); ++i )
                mergee.AddElement() = src[ i ];
            if( modified )
                listener.NotifyChanged( mergee );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Merge
// Created: SLI 2013-06-04
// -----------------------------------------------------------------------------
void Model::Merge( Model& mergee, ModelListener_ABC& listener ) const
{
    apply_visitor( boost::bind( MergeVisitor(), _1, _2, boost::ref( mergee ), boost::ref( listener ) ), content_, mergee.content_ );
}
