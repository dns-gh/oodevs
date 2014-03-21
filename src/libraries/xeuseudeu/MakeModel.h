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
#include <core/ModelListener_ABC.h>
#include <core/UserData.h>

namespace sword
{
namespace test
{
    typedef core::ModelBuilder< Model > ModelBuilder;

    inline std::ostream& operator<<( std::ostream& os, const ModelBuilder& builder )
    {
        return os << *builder.model_;
    }

    inline bool operator==( const core::Model* actual, const ModelBuilder& expected )
    {
        return actual && expected.model_->Check( *actual );
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
}
}

#endif // MODULE_TESTER_MAKE_MODEL_H
