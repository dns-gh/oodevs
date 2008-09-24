// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
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
    class Model;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;
    class Controller;

// =============================================================================
/** @class  ClientConditions
    @brief  ClientConditions
*/
// Created: SBO 2008-06-27
// =============================================================================
class ClientConditions : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ClientConditions( kernel::Controller& controller, const dispatcher::Model& model );
    virtual ~ClientConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
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
    const dispatcher::Model& model_;
    //@}
};

}
}

#endif // __ClientConditions_h_
