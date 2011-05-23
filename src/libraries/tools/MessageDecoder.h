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
    explicit MessageDecoder( Message& message )
    {
        const std::size_t size = message.Size();
        // $$$$ SBO 2009-10-26: Find a way to get rid of new
        char* buffer = new char[ size ];
        message.Read( buffer, size );
        if( ! message_.ParseFromString( std::string( buffer, size ) ) )
            throw std::runtime_error( __FUNCTION__ ": message deserialization failed." );
        delete[] buffer;
    }
    //@}

    //! @name Accessors
    //@{
    operator const T&() const
    {
        return message_;
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
