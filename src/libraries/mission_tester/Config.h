// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "MainFactory_ABC.h"
#include "tools/ExerciseConfig.h"
#include <boost/utility/base_from_member.hpp>

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

namespace xml
{
    class xibufferstream;
}

namespace mission_tester
{
    class Facade;
    class Client;
    class Scheduler_ABC;
    class SchedulerFactory;
    class Timeout;

    enum E_EntityType
    {
        agent = 0,
        automat
    };

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: PHC 2011-04-07
// =============================================================================
class Config : private boost::base_from_member< std::auto_ptr< tools::RealFileLoaderObserver_ABC > >
             , public tools::ExerciseConfig
             , public MainFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Config( int argc, char** argv );
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    virtual void ConfigureLogging( Facade& facade ) const;
    virtual std::auto_ptr< Client > CreateClient( SwordMessageHandler_ABC& handler ) const;
    virtual std::auto_ptr< SchedulerFactory > CreateSchedulerFactory() const;
    virtual std::auto_ptr< Timeout > CreateTimeout() const;
    virtual std::auto_ptr< Exercise > CreateExercise( kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel, Publisher_ABC& publisher ) const;
    virtual std::auto_ptr< Scheduler_ABC > CreateScheduler( SchedulerFactory& factory ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::base_from_member< std::auto_ptr< tools::RealFileLoaderObserver_ABC > > T_FileLoader;
    //@}

private:
    //! @name Member data
    //@{
    std::string configurationFile_;
    std::string host_;
    unsigned short port_;
    std::string login_;
    std::string password_;
    std::string logFile_;
    unsigned int timeout_;
    unsigned int scheduler_;
    bool withFragOrders_;
    std::auto_ptr< xml::xibufferstream > xibs_;
    E_EntityType entityType_;
    //@}
};

}

#endif // __Config_h_
