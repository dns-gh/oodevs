// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LoggerPlugin_h_
#define __LoggerPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "reports/ReportFactory.h"
#include "RcEntityResolver.h"
#include <tools/Path.h>
#include <set>

namespace tools
{
    class SessionConfig;
}

namespace dispatcher
{
    class Model_ABC;
    class Services;
}

namespace kernel
{
    class StaticModel;
}

class MT_FileLogger;

namespace sword
{
    class CrowdsFireDamages;
    class SimToClient;
    class UnitFireDamages;
    class UnitsFireDamages;
}

namespace plugins
{
namespace logger
{
    class ActionsLogger;
    class Simulation;

// =============================================================================
/** @class  LoggerPlugin
    @brief  LoggerPlugin
*/
// Created: LDC 2010-03-17
// =============================================================================
class LoggerPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LoggerPlugin( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel,
                           const tools::SessionConfig& config, const dispatcher::Services& services );
    virtual ~LoggerPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    virtual void Close();
    //@}

private:
    //! @name Helpers
    //@{
    void FormatMission( const char* name, int id, int mission );
    bool Initialize();
    void LogUnitsFireDamages( const sword::UnitsFireDamages& unitsDamages );
    void LogPopulationsFireDamages( const sword::CrowdsFireDamages& populationsDamages );
    void LogDamagesOnTarget( const sword::UnitFireDamages& unitDamages, const kernel::Entity_ABC& target );
    void FormatMessage( const std::string& message, const std::string& level, const std::string& entity,
                        unsigned int id, const std::string& date );
    //@}

private:
    //! @name Member data
    //@{
    const tools::SessionConfig&    sessionConfig_;
    tools::Path                    filename_;
    std::unique_ptr< MT_FileLogger > pLogger_;
    kernel::ObjectTypes            objectTypes_;
    RcEntityResolver               resolver_;
    ReportFactory                  factory_;
    const dispatcher::Model_ABC&   model_;
    const kernel::StaticModel&     staticModel_;
    const dispatcher::Services&    services_;
    std::string                    date_;
    bool                           enabled_;
    bool                           initialized_;
    std::unique_ptr< Simulation >    simulation_;
    std::unique_ptr< ActionsLogger > actions_;
    unsigned int                   nCurrentTick_;
    std::set< int >                missions_;
    //@}
};
}
}

#endif // __LoggerPlugin_h_
