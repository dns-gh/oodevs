// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __plugins_timeline_ActionPublisher_h_
#define __plugins_timeline_ActionPublisher_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
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

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace timeline
{

// =============================================================================
/** @class  ActionPublisher
    @brief  ActionPublisher
*/
// Created: JCR 2010-09-07
// =============================================================================
class ActionPublisher : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionPublisher( const tools::ExerciseConfig& config, const dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& sim );
    virtual ~ActionPublisher();
    //@}

    //! @name Operations
    //@{
    void Execute( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    std::unique_ptr< kernel::EntityResolver_ABC > entities_;
    std::unique_ptr< kernel::CoordinateConverter_ABC > converter_;
    //@}
};

}
}

#endif // __ActionPublisher_h_
