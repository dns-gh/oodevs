// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ClientConditions_h_
#define __ClientConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;
    class Controller;

// =============================================================================
/** @class  ClientConditions
    @brief  Client conditions
*/
// Created: SBO 2008-06-27
// =============================================================================
class ClientConditions : public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientConditions( kernel::Controller& controller, const dispatcher::Model_ABC& model );
    virtual ~ClientConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > UserChose();
    boost::shared_ptr< Condition_ABC > EntitySelected();
    boost::shared_ptr< Condition_ABC > MissionChosen();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientConditions( const ClientConditions& );            //!< Copy constructor
    ClientConditions& operator=( const ClientConditions& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Model_ABC& model_;
    //@}
};

}
}

#endif // __ClientConditions_h_
