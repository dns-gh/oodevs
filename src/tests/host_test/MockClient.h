// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_CLIENT_H
#define MOCK_CLIENT_H

#include "web/Client_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockClient, web::Client_ABC )
    {
        MOCK_METHOD_EXT( Get, 4, web::Client_ABC::T_Response( const std::string&, int, const std::string&, const web::Client_ABC::T_Parameters& ), Get4 );
        MOCK_METHOD_EXT( Get, 5, web::Client_ABC::T_Response( const std::string&, const std::string&, int, const std::string&, const web::Client_ABC::T_Parameters& ), Get5 );
    };

    MOCK_BASE_CLASS( MockResponse, web::Response_ABC )
    {
        MOCK_METHOD( GetStatus, 0 );
        MOCK_METHOD( GetBody, 0 );
    };
};

#endif // MOCK_CLIENT_H
