// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_test_SimTools_h
#define shield_test_SimTools_h

#include "Tools.h"

namespace shield
{
    template< typename L >
    void FillLocation( L* l )
    {
        l->set_type( sword::Location::rectangle );
        FillCoordLatLong( l->mutable_coordinates()->add_elem() );
        FillCoordLatLong( l->mutable_coordinates()->add_elem() );
    }
    template< typename I >
    void FillIntelligence( I* i )
    {
        i->set_name( "name" );
        i->set_nature( "nature" );
        i->set_level( sword::ii );
        i->set_embarked( true );
        FillCoordLatLong( i->mutable_location() );
        i->set_diplomacy( sword::friend_diplo );
        i->mutable_formation()->set_id( 77 );
    }
}

#endif // shield_test_SimTools_h
