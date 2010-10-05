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
#pragma warning( push, 0 )
#include <google/protobuf/Message.h>
#include <google/protobuf/Descriptor.h>
#pragma warning( pop )
#include <sstream>

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
        {
            std::stringstream ss;
            ss  << "Message of type \"" << message.GetDescriptor()->full_name()
                << "\" is missing required fields: " << message.InitializationErrorString();
            throw std::runtime_error( ss.str() );
        }

        // $$$$ SEB 2009-10-27: Alternate implementation, little bit slower but uses stack instead of heap -> less fragmentation
    //    std::string buffer;
    //    if( !message.SerializeToString( &buffer ) )
    //    {
    //        std::stringstream ss;
    //        ss << "Error serializing message of type \"" << message.GetDescriptor()->full_name() << "\"" << std::endl;
    //        throw std::runtime_error( ss.str() );
    //    }
    //    message_.Write( buffer.c_str(), buffer.size() );

        google::protobuf::uint8* buffer = new google::protobuf::uint8[ message.ByteSize() ];
        if( !message.SerializeWithCachedSizesToArray( buffer ) )
        {
            delete[] buffer; // $$$$ MAT : use boost::shared_array
            std::stringstream ss;
            ss << "Error serializing message of type \"" << message.GetDescriptor()->full_name() << "\"" << std::endl;
            throw std::runtime_error( ss.str() );
        }
        message_.Write( (const char*)buffer, message.GetCachedSize() );
        delete[] buffer;
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
