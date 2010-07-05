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
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <google/protobuf/Message.h>
#include <google/protobuf/Descriptor.h>
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
    explicit MessageEncoder( const T& message );
    //@}

    //! @name Accessors
    //@{
    operator const Message&() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MessageEncoder( const MessageEncoder& );            //!< Copy constructor
    MessageEncoder& operator=( const MessageEncoder& ); //!< Assignment operator
    //@}

private:
    Message message_;
};

}

#include "MessageEncoder.inl"

#endif // __MessageEncoder_h_
