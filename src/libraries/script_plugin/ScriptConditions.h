// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScriptConditions_h_
#define __ScriptConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;
    class Controller;

// =============================================================================
/** @class  ScriptConditions
    @brief  ScriptConditions
*/
// Created: SBO 2008-07-02
// =============================================================================
class ScriptConditions : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ScriptConditions( kernel::Controller& controller );
    virtual ~ScriptConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScriptConditions( const ScriptConditions& );            //!< Copy constructor
    ScriptConditions& operator=( const ScriptConditions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > PhaseChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}
}

#endif // __ScriptConditions_h_
