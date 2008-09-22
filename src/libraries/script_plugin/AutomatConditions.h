// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __AutomatConditions_h_
#define __AutomatConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Automat;
}

namespace script
{
    class Condition_ABC;

// =============================================================================
/** @class  AutomatConditions
    @brief  AutomatConditions
*/
// Created: SBO 2008-08-13
// =============================================================================
class AutomatConditions : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatConditions( kernel::Controller& controller );
    virtual ~AutomatConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatConditions( const AutomatConditions& );            //!< Copy constructor
    AutomatConditions& operator=( const AutomatConditions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > MissionStarted();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}

#endif // __AutomatConditions_h_
