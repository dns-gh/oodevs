// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Encodable_H_
#define plugins_hla_Encodable_H_

#include "dispatcher/Logger_ABC.h"
#include <hla/Encodable.h>
#include <stdexcept>

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  Encodable
    @brief  Encodable
*/
// Created: AHC 2012-08-31
// =============================================================================
template< typename T, typename R >
class Encodable : public ::hla::Encodable< T, R >
{
public:
    Encodable( R T::* pMemberData, const std::string& interaction, const std::string& parameter, dispatcher::Logger_ABC& logger )
        : ::hla::Encodable< T, R >( pMemberData )
        , interaction_( interaction )
        , parameter_  ( parameter )
        , logger_     ( logger )
    {
        // NOTHING
    }
    virtual void Encode( ::hla::Serializer_ABC& serializer, const T& object ) const
    {
        ::hla::Encodable< T, R >::Encode( serializer, object );
    }
    virtual void Decode( ::hla::Deserializer_ABC& deserializer, T& object ) const
    {
        try
        {
            ::hla::Encodable< T, R >::Decode( deserializer, object );
        }
        catch( const std::exception& e )
        {
            logger_.LogError( "Interaction '" + interaction_ + "' parameter '" + parameter_ + "' : " + tools::GetExceptionMsg( e ) );
        }
    }
private:
    const std::string interaction_;
    const std::string parameter_;
    dispatcher::Logger_ABC& logger_;
};

}
}

#endif // plugins_hla_Encodable_H_
