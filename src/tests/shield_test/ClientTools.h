// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_test_ClientTools_h
#define shield_test_ClientTools_h

#include "Tools.h"

namespace shield
{
    template< typename L >
    void FillLocation( L* l )
    {
        l->set_type( Common::MsgLocation::rectangle );
        FillCoordLatLong( l->mutable_coordinates()->add_elem() );
        FillCoordLatLong( l->mutable_coordinates()->add_elem() );
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
        p->mutable_read_only_camps()->add_elem()->set_id( 12 );
        p->mutable_read_only_camps()->add_elem()->set_id( 13 );
        p->mutable_read_write_camps()->add_elem()->set_id( 14 );
        p->mutable_read_write_camps()->add_elem()->set_id( 15 );
        p->mutable_read_only_crowds()->add_elem()->set_id( 16 );
        p->mutable_read_only_crowds()->add_elem()->set_id( 17 );
        p->mutable_read_write_crowds()->add_elem()->set_id( 18 );
        p->mutable_read_write_crowds()->add_elem()->set_id( 19 );
    }
}

#endif // shield_test_ClientTools_h
