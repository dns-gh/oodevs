// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Actions_h_
#define __Actions_h_

#include "dispatcher/Registrable_ABC.h"

namespace actions
{
    class ActionFactory_ABC;
    class ParameterFactory_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controller;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  Actions
    @brief  Actions
*/
// Created: AGE 2008-07-16
// =============================================================================
class Actions : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Actions( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& sim );
    virtual ~Actions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Actions( const Actions& );            //!< Copy constructor
    Actions& operator=( const Actions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void IssueOrder( const std::string& name );
    void IssueXmlOrder( const std::string& name );
    //@}

    //! @name Types
    //@{
    struct Publisher;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Publisher > publisher_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory_ABC > factory_;
    std::string file_;
    //@}
};

}
}

#endif // __Actions_h_
