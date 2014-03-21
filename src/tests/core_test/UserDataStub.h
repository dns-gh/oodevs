// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_TEST_USER_DATA_STUB_H
#define CORE_TEST_USER_DATA_STUB_H

#include "core/UserData.h"

namespace core
{
    class UserDataStub
    {
    public:
        UserDataStub()
            : i_()
        {}
        UserDataStub( int i )
            : i_( i )
        {}
        operator int() const
        {
            return i_;
        }
        template< typename Archive >
        void serialize( Archive& a, const unsigned int )
        {
            a & i_;
        }
    private:
        int i_;
    };
}

#endif // CORE_TEST_USER_DATA_STUB_H
