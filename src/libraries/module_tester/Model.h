// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_MODEL_H
#define MODULE_TESTER_MODEL_H

#include "NullModelVisitor.h"
#include <core/Convert.h>
#include <core/ModelPrinter.h>
#include <turtle/check.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4512 4702 4181 )
#elif defined __GNUC__
#   pragma GCC system_header
#endif
#include <boost/variant.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif
#include <vector>
#include <map>

namespace mock
{
    template< typename T >
    bool operator==( const boost::variant< double, std::string >& expected, const T& actual )
    {
        return expected == boost::variant< double, std::string >( actual );
    }
}

namespace sword
{
namespace test
{
class Model : public core::Visitable_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::variant< double, std::string > T_Value;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Model()
        : isAny_( false )
    {}
    Model( const Model& rhs )
        : value_   ( rhs.value_ )
        , data_    ( rhs.data_ )
        , children_( rhs.children_ )
        , elements_( rhs.elements_ )
        , isAny_   ( rhs.isAny_ )
    {}
    //@}

    //! @name Operators
    //@{
    Model& operator=( const Model& rhs )
    {
        value_ = rhs.value_;
        data_ = rhs.data_;
        children_ = rhs.children_;
        elements_ = rhs.elements_;
        isAny_ = rhs.isAny_;
        return *this;
    }

    template< typename T >
    Model& operator=( const T& t )
    {
        value_.reset( new mock::detail::check< T_Value, T >( t ) );
        return *this;
    }
    Model& operator=( const mock::constraint< mock::detail::any >& /*any*/ )
    {
        isAny_ = true;
        return *this;
    }
    Model& operator=( const char* const c )
    {
        value_.reset( new mock::detail::check< T_Value, std::string >( c ) );
        return *this;
    }

    Model& operator[]( const std::string& key )
    {
        if( key.empty() )
            return *this;
        Model& child = children_[ Root( key ) ];
        return child[ Leaf( key ) ];
    }
    Model& operator[]( const char* key )
    {
        return operator[]( std::string( key ) );
    }
    Model& operator[]( unsigned int key )
    {
        return children_[ boost::lexical_cast< std::string >( key ) ];
    }

    bool Check( const core::Visitable_ABC& actual ) const
    {
        bool valid = true;
        NodeValidator( *this, actual, valid );
        return valid;
    }
    bool IsAny() const
    {
        return isAny_;
    }
    //@}

    //! @name Operations
    //@{
    Model& AddElement()
    {
        elements_.push_back( Model() );
        return elements_.back();
    }
    void SetData( boost::shared_ptr< core::UserData_ABC > data )
    {
        if( data && data->Get() )
            data_ = data;
        else
            data_.reset();
    }

    virtual void Accept( core::ModelVisitor_ABC& visitor ) const
    {
        if( value_ )
            visitor.Visit( boost::lexical_cast< std::string >( *value_ ) );
        if( data_ )
            visitor.Visit( data_ );
        for( T_Children::const_iterator it = children_.begin(); it != children_.end(); ++it )
            visitor.Visit( it->first, it->second );
        for( std::size_t i = 0; i < elements_.size(); ++i )
            visitor.Visit( elements_[ i ] );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Model > T_Children;

    struct ValueValidator : NullModelVisitor
    {
        ValueValidator( const Model& model, const core::Visitable_ABC& actual, bool& valid )
            : model_  ( model )
            , valid_  ( valid )
            , visited_( false )
        {
            actual.Accept( *this );
            if( ! visited_ && model_.value_ )
                valid_ = false;
        }
#define VISIT( type ) \
        virtual void Visit( type value ) \
        { \
            visited_ = true; \
            if( ! model_.value_ || ! (*model_.value_)( value ) ) \
                valid_ = false; \
        }
        VISIT( int64_t )
        VISIT( uint64_t )
        VISIT( double )
        VISIT( const std::string& )
#undef VISIT

        const Model& model_;
        bool& valid_;
        bool visited_;
    };

    struct ChildrenValidator : NullModelVisitor
    {
        ChildrenValidator( const Model& model, const core::Visitable_ABC& actual, bool& valid )
            : model_  ( model )
            , valid_  ( valid )
            , visited_( 0 )
        {
            actual.Accept( *this );
            if( visited_ != model.children_.size() )
                valid = false;
        }

        virtual void Visit( const std::string& key, const core::Visitable_ABC& child )
        {
            ++visited_;
            T_Children::const_iterator it = model_.children_.find( key );
            if( it == model_.children_.end() )
                valid_ = false;
            else
                NodeValidator( it->second, child, valid_ );
        }
        virtual void Visit( unsigned int key, const core::Visitable_ABC& child )
        {
            Visit( boost::lexical_cast< std::string >( key ), child );
        }

        const Model& model_;
        bool& valid_;
        std::size_t visited_;
    };
    
    struct ElementValidator : NullModelVisitor
    {
        ElementValidator( const Model& model, const core::Visitable_ABC& actual, bool& valid )
            : model_( model )
            , valid_( valid )
            , count_( 0 )
        {
            actual.Accept( *this );
            if( count_ != model_.elements_.size() )
                valid = false;
        }

        virtual void Visit( const core::Visitable_ABC& element )
        {
            if( count_ >= model_.elements_.size() )
                valid_ = false;
            else
                NodeValidator( model_.elements_[ count_++ ], element, valid_ );
        }

        const Model& model_;
        bool& valid_;
        std::size_t count_;
    };
    
    struct UserDataValidator : NullModelVisitor
    {
        UserDataValidator( const Model& model, const core::Visitable_ABC& actual, bool& valid )
            : model_  ( model )
            , valid_  ( valid )
            , visited_( false )
        {
            actual.Accept( *this );
            if( ! visited_ && model_.data_ )
                valid_ = false;
        }
        virtual void Visit( boost::shared_ptr< core::UserData_ABC > data )
        {
            visited_ = true;
            if( ! model_.data_ || data->Get() != model_.data_->Get() )
                valid_ = false;
        }

        const Model& model_;
        bool& valid_;
        bool visited_;
    };

    struct NodeValidator : NullModelVisitor
    {
        NodeValidator( const Model& model, const core::Visitable_ABC& actual, bool& valid )
            : model_( model )
            , valid_( valid )
        {
            if( model_.IsAny() )
                return;
            ValueValidator( model, actual, valid );
            ChildrenValidator( model, actual, valid );
            ElementValidator( model, actual, valid );
            UserDataValidator( model, actual, valid );
        }

        const Model& model_;
        bool& valid_;
    };
    //@}

    //! @name Helpers
    //@{
    std::string Root( const std::string& key ) const
    {
        std::size_t pos = key.find( '/' );
        if( pos == std::string::npos )
            return key;
        return key.substr( 0, pos );
    }
    std::string Leaf( const std::string& key ) const
    {
        std::size_t pos = key.find( '/' );
        if( pos == std::string::npos )
            return "";
        return key.substr( pos + 1 );
    }
    //@}

    //! @name Member data
    //@{
    boost::shared_ptr< mock::detail::check_base< T_Value > > value_;
    boost::shared_ptr< core::UserData_ABC > data_;
    std::vector< Model > elements_;
    T_Children children_;
    bool isAny_;
    //@}
};

inline std::ostream& operator<<( std::ostream& os, const Model& model )
{
    core::ModelPrinter( model, os, false, 2 );
    return os;
}

}
}

#endif // MODULE_TESTER_MODEL_H
