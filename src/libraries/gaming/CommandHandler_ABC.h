// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandHandler_ABC_h_
#define __CommandHandler_ABC_h_

class Command;

// =============================================================================
/** @class  CommandHandler_ABC
    @brief  Command handler interface
*/
// Created: AGE 2008-06-12
// =============================================================================
class CommandHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CommandHandler_ABC() {};
    virtual ~CommandHandler_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command ) = 0;
    //@}
};

#endif // __CommandHandler_ABC_h_
