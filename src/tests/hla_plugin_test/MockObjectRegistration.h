// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockObjectRegistration_h
#define hla_plugin_MockObjectRegistration_h

#include <hla/ObjectRegistration_ABC.h>

namespace hla
{
// =============================================================================
/** @class  MockObjectRegistration
    @brief  Mock object registration
*/
// Created: SLI 2011-07-26
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockObjectRegistration, ::hla::ObjectRegistration_ABC< T > )
{
    MOCK_METHOD_EXT_TPL( Create, 2, T&( const ::hla::ObjectIdentifier&, const std::string& ), Create )
    MOCK_METHOD_EXT_TPL( Destroy, 1, void( T& ), Destroy )
    MOCK_METHOD_EXT_TPL( Reflected, 1, void( T& ), Reflected )
    MOCK_METHOD_EXT_TPL( RequestConfirmDivestiture, 1, bool( const T& ), RequestConfirmDivestiture )
    MOCK_METHOD_EXT_TPL( OwnershipAcquisitionNotification, 1, void( const T& ), OwnershipAcquisitionNotification )
    MOCK_METHOD_EXT_TPL( RequestOwnershipAssumption, 1, bool( const T& ), RequestOwnershipAssumption )
};

}

#endif // hla_plugin_MockObjectRegistration_h
