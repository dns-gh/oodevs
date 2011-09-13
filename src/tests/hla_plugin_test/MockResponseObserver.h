// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockResponseObserver_h
#define MockResponseObserver_h

#include "hla_plugin/ResponseObserver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockResponseObserver
    @brief  Mock Response observer
*/
// Created: SLI 2011-09-09
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockResponseObserver, ResponseObserver_ABC< T > )
{
    MOCK_METHOD_EXT_TPL( Notify, 2, void( const T&, const std::string& ), Notify );
};

}
}

#endif // MockResponseObserver_h
