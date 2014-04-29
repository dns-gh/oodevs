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
    MOCK_METHOD_TPL( Create, 2, T&( const ::hla::ObjectIdentifier&, const std::string& ) )
    MOCK_METHOD_TPL( Destroy, 1, void( T& ) )
    MOCK_METHOD_TPL( Reflected, 1, void( T& ) )
    MOCK_METHOD_TPL( RequestConfirmDivestiture, 3, bool( const ::hla::ObjectIdentifier&, const T&, const T_AttributeIdentifiers& ) )
    MOCK_METHOD_TPL( OwnershipAcquisitionNotification, 4, void( const ::hla::ObjectIdentifier&, const T&, const T_AttributeIdentifiers&, const ::hla::VariableLengthData& ) )
    MOCK_METHOD_TPL( RequestOwnershipAssumption, 4, bool( const ::hla::ObjectIdentifier&, const T&, const T_AttributeIdentifiers&, const ::hla::VariableLengthData& ) )
    MOCK_METHOD_TPL( RequestOwnershipRelease, 4, void( const ::hla::ObjectIdentifier&, const T&, const T_AttributeIdentifiers&, const ::hla::VariableLengthData& ) )
};

}

#endif // hla_plugin_MockObjectRegistration_h
