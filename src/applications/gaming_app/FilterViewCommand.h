// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FilterViewCommand_h_
#define __FilterViewCommand_h_

#include "gaming/CommandHandler_ABC.h"

class CommandHandler;
class ProfileFilter;

// =============================================================================
/** @class  FilterViewCommand
    @brief  FilterViewCommand
*/
// Created: SBO 2009-03-04
// =============================================================================
class FilterViewCommand : public CommandHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FilterViewCommand( CommandHandler& handler, ProfileFilter& filter );
    virtual ~FilterViewCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FilterViewCommand( const FilterViewCommand& );            //!< Copy constructor
    FilterViewCommand& operator=( const FilterViewCommand& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    CommandHandler& handler_;
    ProfileFilter& filter_;
    //@}
};

#endif // __FilterViewCommand_h_
