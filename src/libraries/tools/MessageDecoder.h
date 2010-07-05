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
    explicit MessageDecoder( Message& message );
    //@}

    //! @name Accessors
    //@{
    operator const T&() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageDecoder( const MessageDecoder& );            //!< Copy constructor
    MessageDecoder& operator=( const MessageDecoder& ); //!< Assignment operator
    //@}

private:
    T message_;
};

}

#include "MessageDecoder.inl"

#endif // __MessageDecoder_h_
