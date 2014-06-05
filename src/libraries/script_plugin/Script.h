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

#include <boost/noncopyable.hpp>

namespace directia
{
namespace brain
{
    class Brain;
}
}

namespace dispatcher
{
    class Registrable_ABC;
}

namespace tools
{
    class Path;
}

namespace plugins
{
namespace script
{
    class FiniteStateMachine;

// =============================================================================
/** @class  Script
    @brief  Script
*/
// Created: AGE 2008-06-12
// =============================================================================
class Script : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Script( const tools::Path& file, dispatcher::Registrable_ABC& registrables );
    virtual ~Script();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< directia::brain::Brain > brain_;
    std::unique_ptr< FiniteStateMachine > fsm_;
    //@}
};

}
}

#endif // __Script_h_
