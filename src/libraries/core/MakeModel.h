// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_MAKE_MODEL_H
#define CORE_MAKE_MODEL_H

#include "ModelBuilder.h"

namespace core
{
    template< typename Key, typename T >
    ModelBuilder< Model > MakeModel( const Key& key, const T& t )
    {
        return ModelBuilder< Model >( key, t );
    }

    inline ModelBuilder< Model > MakeModel()
    {
        return ModelBuilder< Model >();
    }

    template< typename T >
    ModelBuilder< Model > MakeModel( const T& t )
    {
        return ModelBuilder< Model >( t );
    }

    template< typename Data >
    boost::shared_ptr< UserData_ABC > MakeUserData( Data userData )
    {
        return boost::make_shared< UserData< Data > >( userData );
    }
}

#endif // CORE_MAKE_MODEL_H
