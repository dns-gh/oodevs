// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScriptCommands_h_
#define __ScriptCommands_h_

#include "dispatcher/Registrable_ABC.h"

namespace kernel
{
    class Controller;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ScriptCommands
    @brief  ScriptCommands
*/
// Created: SBO 2008-07-02
// =============================================================================
class ScriptCommands : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ScriptCommands( kernel::Controller& controller );
    virtual ~ScriptCommands();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScriptCommands( const ScriptCommands& );            //!< Copy constructor
    ScriptCommands& operator=( const ScriptCommands& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ChangePhase( const std::string& phase );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}
}

#endif // __ScriptCommands_h_
