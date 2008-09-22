// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConditionFacade_h_
#define __ConditionFacade_h_

#include "dispatcher/CompositeRegistrable.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Model;
}

namespace script
{
    class Condition_ABC;

// =============================================================================
/** @class  ConditionFacade
    @brief  ConditionFacade
*/
// Created: AGE 2008-06-12
// =============================================================================
class ConditionFacade : public dispatcher::CompositeRegistrable
{

public:
    //! @name Constructors/Destructor
    //@{
             ConditionFacade( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const dispatcher::Model& model );
    virtual ~ConditionFacade();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > Once();
    boost::shared_ptr< Condition_ABC > Timer( float seconds );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}

#endif // __ConditionFacade_h_
