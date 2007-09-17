// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MessageLoader_ABC_h_
#define __MessageLoader_ABC_h_

#include <boost/function.hpp>

namespace dispatcher
{
    class MessageHandler_ABC;

// =============================================================================
/** @class  MessageLoader_ABC
    @brief  Message loader definition
*/
// Created: AGE 2007-09-17
// =============================================================================
class MessageLoader_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::function0< void > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MessageLoader_ABC() {};
    virtual ~MessageLoader_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool         LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() ) = 0;
    virtual unsigned int LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() ) = 0;
    virtual unsigned int GetTickNumber() const = 0;
    //@}
};

}

#endif // __MessageLoader_ABC_h_
