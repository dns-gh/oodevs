// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __plugins_timeline_ScenarioManager_h_
#define __plugins_timeline_ScenarioManager_h_

#include "ResponseHandler_ABC.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace timeline
{
    class Task;
    class ActionPublisher;
    class Publisher_ABC;

// =============================================================================
/** @class  Scenario
    @brief  Scenario
*/
// Created: JCR 2010-09-07
// =============================================================================
class ScenarioManager : public ResponseHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ScenarioManager( ActionPublisher& actions );
    virtual ~ScenarioManager();
    //@}

    //! @name Operations
    //@{
    void Update( Publisher_ABC& publisher );
    void Handle( xml::xistream& xis );
    //@}

private:
    typedef std::map< long, boost::shared_ptr< Task > > T_TaskMap;

private:
    //! @name Copy/Assignment
    //@{
    ScenarioManager( const ScenarioManager& );            //!< Copy constructor
    ScenarioManager& operator=( const ScenarioManager& ); //!< Assignment operator
    //@}

    //! @name Helper
    //@{
    void ReadTask( xml::xistream& xis );
    void ReadSchedule( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_TaskMap       tasks_;
    ActionPublisher& actions_;
    std::string      timestamp_;
    //@}
};

}
}

#endif // __plugins_timeline_ScenarioManager_h_
