// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageEncoder_h_
#define __MessageEncoder_h_

#include "Message.h"
#include <boost/shared_array.hpp>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#pragma warning( pop )

namespace tools
{
// =============================================================================
/** @class  MessageEncoder
    @brief  MessageEncoder
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T >
class MessageEncoder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MessageEncoder( const T& message )
    {
        if( !message.IsInitialized() )
            throw std::runtime_error( "Message of type \"" + message.GetDescriptor()->full_name()
                + "\" is missing required fields: " + message.InitializationErrorString() );
        boost::shared_array< google::protobuf::uint8 > buffer( new google::protobuf::uint8[ message.ByteSize() ] );
        if( !message.SerializeWithCachedSizesToArray( buffer.get() ) )
            throw std::runtime_error( "Error serializing message of type \"" + message.GetDescriptor()->full_name() + '"' );
        message_.Write( (const char*)buffer.get(), message.GetCachedSize() );
    }
    //@}

    //! @name Accessors
    //@{
    operator const Message&() const
    {
        return message_;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageEncoder( const MessageEncoder& );            //!< Copy constructor
    MessageEncoder& operator=( const MessageEncoder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Message message_;
    //@}
};

}

#endif // __MessageEncoder_h_
