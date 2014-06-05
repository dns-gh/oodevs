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
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controller;
    class EntityResolver_ABC;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace script
{
    class ActionScheduler;
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
             Actions( kernel::Controller& controller,
                      const tools::ExerciseConfig& config,
                      const dispatcher::Model_ABC& model,
                      dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~Actions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    void IssueOrder( const std::string& name );
    void IssueOrderFromFile( const std::string& name, const std::string& filename );
    void IssueXmlOrder( const std::string& content );
    void StartScheduler( const std::string& filename );
    void StopScheduler();
    void Send( xml::xistream& xis );
    void Read( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< ActionScheduler > > T_Schedulers;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    kernel::Controller& controller_;
    std::unique_ptr< kernel::EntityResolver_ABC > entities_;
    std::unique_ptr< kernel::CoordinateConverter_ABC > converter_;
    T_Schedulers schedulers_;
    //@}
};

}
}

#endif // __Actions_h_
