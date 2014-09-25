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
#include <boost/noncopyable.hpp>

namespace kernel
{
    class OptionsController;
}

class CommandHandler;

// =============================================================================
/** @class  ChangeOptionCommand
    @brief  ChangeOptionCommand
*/
// Created: AGE 2008-06-25
// =============================================================================
class ChangeOptionCommand : public CommandHandler_ABC
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ChangeOptionCommand( CommandHandler& handler, kernel::OptionsController& options );
    virtual ~ChangeOptionCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
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
    kernel::OptionsController& options_;
    //@}
};

#endif // __ChangeOptionCommand_h_
