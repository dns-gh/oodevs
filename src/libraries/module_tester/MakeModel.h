// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_MAKE_MODEL_H
#define MODULE_TESTER_MAKE_MODEL_H

#include "Model.h"
#include "Tools.h" // $$$$ MCO 2012-04-17: ?
#include <core/Model.h>
#include <core/ModelBuilder.h>
#include <core/UserData.h>

namespace sword
{
namespace test
{
    struct Assign
    {
        Assign( const Model& lhs, Model& rhs )
        {
            rhs = lhs;
        }
    };
    typedef core::ModelBuilder< Model, Assign > ModelBuilder;

    inline bool operator==( const SWORD_Model* actual, const ModelBuilder& expected )
    {
        return actual && expected.model_->Check( *core::Convert( actual ) );
    }

    inline std::ostream& operator<<( std::ostream& os, const ModelBuilder& builder )
    {
        return os << *builder.model_;
    }

    inline ModelBuilder MakeModel()
    {
        return ModelBuilder();
    }
    template< typename Value >
    ModelBuilder MakeModel( const Value& value )
    {
        return ModelBuilder( value );
    }
    template< typename Key, typename Value >
    ModelBuilder MakeModel( const Key& key, const Value& value )
    {
        return ModelBuilder( key, value );
    }

    template< typename Data >
    boost::shared_ptr< core::UserData_ABC > MakeUserData( Data data )
    {
        return boost::make_shared< core::UserData< Data > >( data );
    }

    template< typename T >
    ModelBuilder MarkForRemove( const T& t )
    {
        return ModelBuilder( t ).MarkForRemove();
    }
    inline ModelBuilder MarkForRemove()
    {
        return ModelBuilder().MarkForRemove();
    }
    inline ModelBuilder MarkForRemove( boost::shared_ptr< core::UserData_ABC > data )
    {
        return ModelBuilder( data ).MarkForRemove();
    }
    inline ModelBuilder MarkForRemove( ModelBuilder& builder )
    {
        return builder.MarkForRemove();
    }
}
}

#endif // MODULE_TESTER_MAKE_MODEL_H
