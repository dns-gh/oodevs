// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageDecoder_h_
#define __MessageDecoder_h_

#include "Message.h"

namespace tools
{
// =============================================================================
/** @class  MessageDecoder
    @brief  MessageDecoder
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T >
class MessageDecoder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MessageDecoder( const Message& message )
    {
        if( ! message_.ParseFromArray( message.Data(), message.Size() ) )
            throw std::runtime_error( "Message deserialization failed" );
    }
    //@}

    //! @name Accessors
    //@{
    operator const T&() const
    {
        return message_;
    }

    std::string DebugString() const
    {
        return message_.ShortDebugString();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageDecoder( const MessageDecoder& );            //!< Copy constructor
    MessageDecoder& operator=( const MessageDecoder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T message_;
    //@}
};

}

#endif // __MessageDecoder_h_
