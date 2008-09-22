// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Script_h_
#define __Script_h_

namespace directia
{
    class Brain;
}

namespace dispatcher
{
    class Registrable_ABC;
}

namespace script
{

class FiniteStateMachine;

// =============================================================================
/** @class  Script
    @brief  Script
*/
// Created: AGE 2008-06-12
// =============================================================================
class Script
{

public:
    //! @name Constructors/Destructor
    //@{
             Script( const std::string& file, dispatcher::Registrable_ABC& registrables );
    virtual ~Script();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Script( const Script& );            //!< Copy constructor
    Script& operator=( const Script& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string file_;
    std::auto_ptr< directia::Brain > brain_;
    std::auto_ptr< FiniteStateMachine > fsm_;
    //@}
};

}

#endif // __Script_h_
