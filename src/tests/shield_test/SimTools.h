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
        i->set_diplomacy( sword::friendly );
        i->mutable_formation()->set_id( 77 );
    }
    template< typename P >
    void FillProfile( P* p )
    {
        FillProfileDescription( p );
        p->mutable_read_only_formations()->add_elem()->set_id( 3 );
        p->mutable_read_only_formations()->add_elem()->set_id( 5 );
        p->mutable_read_write_formations()->add_elem()->set_id( 6 );
        p->mutable_read_write_formations()->add_elem()->set_id( 7 );
        p->mutable_read_only_automates()->add_elem()->set_id( 8 );
        p->mutable_read_only_automates()->add_elem()->set_id( 9 );
        p->mutable_read_write_automates()->add_elem()->set_id( 10 );
        p->mutable_read_write_automates()->add_elem()->set_id( 11 );
        p->mutable_read_only_parties()->add_elem()->set_id( 12 );
        p->mutable_read_only_parties()->add_elem()->set_id( 13 );
        p->mutable_read_write_parties()->add_elem()->set_id( 14 );
        p->mutable_read_write_parties()->add_elem()->set_id( 15 );
        p->mutable_read_only_crowds()->add_elem()->set_id( 16 );
        p->mutable_read_only_crowds()->add_elem()->set_id( 17 );
        p->mutable_read_write_crowds()->add_elem()->set_id( 18 );
        p->mutable_read_write_crowds()->add_elem()->set_id( 19 );
    }
}

#endif // shield_test_SimTools_h
