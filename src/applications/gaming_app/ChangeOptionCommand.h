// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChangeOptionCommand_h_
#define __ChangeOptionCommand_h_

#include "gaming/CommandHandler_ABC.h"

namespace kernel
{
    class Options;
}

class CommandHandler;

// =============================================================================
/** @class  ChangeOptionCommand
    @brief  ChangeOptionCommand
*/
// Created: AGE 2008-06-25
// =============================================================================
class ChangeOptionCommand : public CommandHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ChangeOptionCommand( CommandHandler& handler, kernel::Options& options );
    virtual ~ChangeOptionCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChangeOptionCommand( const ChangeOptionCommand& );            //!< Copy constructor
    ChangeOptionCommand& operator=( const ChangeOptionCommand& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ChangeOption( char type, const std::string& name, const std::string& value );
    template< typename T >
    void SetOption( const std::string& name, const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    CommandHandler& handler_;
    kernel::Options& options_;
    //@}
};

#endif // __ChangeOptionCommand_h_
