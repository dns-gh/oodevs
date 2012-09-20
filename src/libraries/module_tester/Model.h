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
#include "ModelConstraint_ABC.h"
#include "IsEmptyModelVisitor.h"
#include <core/Model.h>
#include <turtle/detail/check.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <iomanip>
#include <vector>
#include <map>

namespace sword
{
namespace test
{
// =============================================================================
/** @class  Model
    @brief  Mock model
*/
// Created: MCO 2012-09-20
// =============================================================================
class Model
{
public:
    //! @name Constructors/Destructor
    //@{
    Model()
        : constraint_( new EmptyConstraint() )
        , removed_   ( false )
    {}
    Model( const Model& rhs )
        : constraint_( rhs.constraint_ )
        , data_      ( rhs.data_ )
        , children_  ( rhs.children_ )
        , elements_  ( rhs.elements_ )
        , removed_   ( rhs.removed_ )
    {}
    //@}

    //! @name Operators
    //@{
    Model& operator=( const Model& rhs )
    {
        constraint_ = rhs.constraint_;
        data_ = rhs.data_;
        children_ = rhs.children_;
        elements_ = rhs.elements_;
        removed_ = rhs.removed_;
        return *this;
    }

    template< typename T >
    Model& operator=( const T& t )
    {
        constraint_.reset( new ConstraintWrapper< T >( t ) );
        return *this;
    }
    Model& operator=( const char* c )
    {
        constraint_.reset( new ConstraintWrapper< const char* >( c ) );
        return *this;
    }

    Model& Model::operator[]( const std::string& key )
    {
        if( key.empty() )
            return *this;
        constraint_.reset( new ChildrenConstraint() );
        return children_[ Root( key ) ][ Leaf( key ) ];
    }
    Model& operator[]( const char* key )
    {
        return operator[]( std::string( key ) );
    }
    Model& operator[]( unsigned int key )
    {
        return operator[]( boost::lexical_cast< std::string >( key ) );
    }

    bool Check( const core::Model& actual ) const
    {
        try
        {
            return CheckRemove( actual ) && constraint_->Check( *this, actual );
        }
        catch( ... )
        {
        }
        return false;
    }
    //@}

    //! @name Operations
    //@{
    Model& AddElement()
    {
        constraint_.reset( new ElementsConstraint() );
        elements_.push_back( Model() );
        return elements_.back();
    }
    void SetData( boost::shared_ptr< core::UserData_ABC > data )
    {
        if( ! data )
            throw std::invalid_argument( "empty user data are not allowed in a model" );
        constraint_.reset( new UserDataConstraint() );
        data_ = data;
    }
    Model& MarkForRemove()
    {
        removed_ = true;
        return *this;
    }

    void Serialize( std::ostream& os, std::size_t indent = 0 ) const
    {
        constraint_->Serialize( *this, os, indent );
    }
    //@}

private:
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

    bool CheckRemove( const core::Model& actual ) const
    {
        struct RemoveCheck : NullModelVisitor
        {
            RemoveCheck( const core::Model& model )
                : removed_( false )
            {
                model.Accept( *this );
            }
            virtual void MarkForRemove()
            {
                removed_ = true;
            }
            bool removed_;
        };
        RemoveCheck check( actual );
        return removed_ == check.removed_;
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Model > T_Children;
    typedef T_Children::const_iterator   CIT_Children;
    typedef std::vector< Model >         T_Elements;
    typedef T_Elements::const_iterator CIT_Elements;

    template< typename T >
    class ConstraintWrapper : public ModelConstraint_ABC
    {
    public:
        ConstraintWrapper( T t )
            : check_( t )
        {}
        virtual bool Check( const Model& /*expected*/, const core::Model& actual )
        {
            return check_( actual );
        }
        virtual void Serialize( const Model& /*expected*/, std::ostream& os, std::size_t /*indent*/ ) const
        {
            os << check_;
        }
    private:
        mock::detail::check< const core::Model&, T > check_;
    };

    class EmptyConstraint : public ModelConstraint_ABC
    {
    public:
        virtual bool Check( const Model& /*expected*/, const core::Model& actual )
        {
            return IsEmptyModelVisitor( actual );
        }
        virtual void Serialize( const Model& /*expected*/, std::ostream& os, std::size_t /*indent*/ ) const
        {
            os << "<empty>";
        }
    };

    class UserDataConstraint : public ModelConstraint_ABC
    {
    public:
        virtual bool Check( const Model& expected, const core::Model& actual )
        {
            return actual.GetData()->Get() == expected.data_->Get();
        }
        virtual void Serialize( const Model& expected, std::ostream& os, std::size_t /*indent*/ ) const
        {
            os << expected.data_->Get();
        }
    };

    class ChildrenConstraint : public ModelConstraint_ABC, private NullModelVisitor
    {
    public:
        virtual bool Check( const Model& expected, const core::Model& actual )
        {
            expected_ = &expected;
            count_ = 0;
            valid_ = true;
            actual.Accept( *this );
            return count_ == expected.children_.size() && valid_;
        }
        virtual void Serialize( const Model& expected, std::ostream& os, std::size_t indent ) const
        {
            os << shift( indent ) << "{" << std::endl;
            for( CIT_Children it = expected.children_.begin(); it != expected.children_.end(); )
            {
                os << shift( indent + 2 ) << it->first << ": ";
                it->second.Serialize( os, indent + 2 );
                if( ++it != expected.children_.end() )
                    os << "," << std::endl;
            }
            os << std::endl << shift( indent ) << "}";
        }
    private:
        virtual void Visit( const std::string& key, const core::Model& child )
        {
            if( ! valid_ )
                return;
            CIT_Children it = expected_->children_.find( key );
            valid_ = it != expected_->children_.end() && it->second.Check( child );
            ++count_;
        }
        virtual void Visit( unsigned int key, const core::Model& child )
        {
            Visit( boost::lexical_cast< std::string >( key ), child );
        }
    private:
        const Model* expected_;
        std::size_t count_;
        bool valid_;
    };

    class ElementsConstraint : public ModelConstraint_ABC
    {
    public:
        virtual bool Check( const Model& expected, const core::Model& actual )
        {
            if( actual.GetSize() != expected.elements_.size() )
                return false;
            for( std::size_t i = 0; i < expected.elements_.size(); ++i )
                if( ! expected.elements_[ i ].Check( actual.GetElement( i ) ) )
                    return false;
            return true;
        }
        virtual void Serialize( const Model& expected, std::ostream& os, std::size_t indent ) const
        {
            os << shift( indent ) << '[' << std::endl;
            for( CIT_Elements it = expected.elements_.begin(); it != expected.elements_.end(); )
            {
                it->Serialize( os, indent + 2 );
                if( ++it != expected.elements_.end() )
                    os << "," << std::endl;
            }
            os << std::endl << shift( indent ) << ']';
        }
    };

    struct shift
    {
        shift( std::size_t indent )
            : indent_( indent )
        {}
        friend std::ostream& operator<<( std::ostream& os, const shift& s )
        {
            return os << std::setw( s.indent_ ) << "";
        }
        std::size_t indent_;
    };

private:
    //! @name Member data
    //@{
    boost::shared_ptr< ModelConstraint_ABC > constraint_;
    boost::shared_ptr< core::UserData_ABC > data_;
    T_Children children_;
    T_Elements elements_;
    bool removed_;
    //@}
};

inline std::ostream& operator<<( std::ostream& os, const Model& model )
{
    model.Serialize( os );
    return os;
}

}
}

#endif // MODULE_TESTER_MODEL_H
