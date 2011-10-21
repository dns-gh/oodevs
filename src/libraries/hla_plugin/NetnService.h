// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnService_h
#define plugins_hla_NetnService_h

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NetnService
    @brief  Netn offer convoy
*/
// Created: SLI 2011-10-11
// =============================================================================
template< typename T >
void RegisterNetnService( ::hla::Interaction< T >& interaction )
{
    interaction.Register( "ServiceID"  , ::hla::CreateParameter< T >( &T::serviceId ) );
    interaction.Register( "Consumer"   , ::hla::CreateParameter< T >( &T::consumer ) );
    interaction.Register( "Provider"   , ::hla::CreateParameter< T >( &T::provider ) );
    interaction.Register( "ServiceType", ::hla::CreateParameter< T >( &T::serviceType ) );
}

}
}

#endif // plugins_hla_NetnService_h
