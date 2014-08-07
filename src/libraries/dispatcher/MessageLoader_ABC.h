// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageLoader_ABC_h_
#define __MessageLoader_ABC_h_

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
    typedef std::function< void() > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MessageLoader_ABC() {}
    virtual ~MessageLoader_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() ) = 0;
    virtual void LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() ) = 0;
    virtual unsigned int GetTickNumber() const = 0;
    virtual unsigned int GetFirstTick() const = 0;
    virtual unsigned int FindKeyFrame( unsigned int frameNumber ) = 0;
    //@}
};

}

#endif // __MessageLoader_ABC_h_
