// *****************************************************************************
//
// $Created: NLD 2003-11-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Config.h $
// $Author: Nld $
// $Modtime: 15/04/05 17:05 $
// $Revision: 7 $
// $Workfile: MIL_Config.h $
//
// *****************************************************************************

#ifndef __MIL_Config_h_
#define __MIL_Config_h_

#include "tools/SessionConfig.h"
#include <boost/optional.hpp>
#include <map>
#include <vector>

namespace xml
{
    class xistream;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

// =============================================================================
// Created: NLD 2003-11-21
// =============================================================================
class MIL_Config : public virtual tools::SessionConfig
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_Config( const boost::shared_ptr< tools::RealFileLoaderObserver_ABC >& observer );
    virtual ~MIL_Config();
    //@}

    //! @name Accessors
    //@{
    bool               CheckAutomateComposition    () const;
    bool               UseDecDebug                 () const;
    bool               UsePathDebug                () const;
    bool               IsDecisionalProfilingEnabled() const;
    bool               IsDecisionalLoggerEnabled   () const;
    bool               IsHookProfilingEnabled      () const;
    bool               IsCommandProfilingEnabled   () const;
    bool               IsSaveCheckpointTestMode    () const;
    bool               IsDispatcherEmbedded        () const;
    const std::string& GetNetworkAddress           () const;
    unsigned long      GetNetworkTimeout           () const;
    bool               IsThreadedNetwork           () const;
    bool               EnableTestCommands          () const;
    // Enable triggering of random breakdowns on equipments, mostly for tests.
    bool               EnableRandomBreakdowns      () const;

    bool               UseNetworkLogger            () const;
    unsigned short     GetNetworkLoggerPort        () const;
    unsigned int       GetCheckPointsFrequency     () const;
    unsigned int       GetCheckPointsKept          () const;
    unsigned int       GetTimeStep                 () const;
    unsigned int       GetTimeFactor               () const;
    unsigned int       GetEndTick                  () const;
    unsigned int       GetTickLatency              () const;
    bool               GetPausedAtStartup          () const;
    unsigned int       GetPathFinderThreads        () const;
    boost::optional< unsigned int > GetPathFinderMaxComputationTime() const;
    const tools::Path& GetCheckpointNameTestMode   () const;
    const tools::Path& GetIntegrationDir           () const;
    const tools::Path& GetPathfindDir              () const;
    const std::string& GetPathfindFilter           () const;

    int                GetRandomSeed               () const;
    const std::vector< bool >& GetRandomGaussian   () const;
    const std::vector< double>& GetRandomDeviation () const;
    const std::vector< double >& GetRandomMean     () const;
    unsigned int       GetGarbageCollectorPause    () const;
    unsigned int       GetGarbageCollectorStepMul  () const;
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );

    tools::Path BuildCheckpointChildFile( const tools::Path& file, tools::Path name = "" ) const;

    virtual tools::Path GetOrbatFile() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadSessionFile            ( const tools::Path& file );
    void ReadSessionXml             ( xml::xistream& xis );
    void ReadCheckPointConfiguration( xml::xistream& xis );
    void ReadDebugConfiguration     ( xml::xistream& xis );
    void ConfigureRandom            ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int   checkPointsKept_;
    unsigned int   checkPointsFrequency_;
    unsigned int   timeStep_;
    unsigned int   timeFactor_;
    unsigned int   endTick_;
    unsigned int   tickLatency_;
    unsigned int   pathFinderThreads_;
    boost::optional< unsigned int > pathFinderMaxComputationTime_;
    unsigned short networkLoggerPort_;
    std::string    networkAddress_;
    unsigned long  networkTimeOut_;
    bool           bCheckPointOrbat_;
    bool           bCheckAutomateComposition_;
    bool           bUseDecDebug_;
    bool           bUsePathDebug_;
    bool           bUseNetworkLogger_;
    bool           bDecisionalProfilingEnabled_;
    bool           bDecisionalLoggerEnabled_;
    bool           bHookProfilingEnabled_;
    bool           bCommandProfilingEnabled_;
    bool           bTestMode_;
    bool           bSaveCheckpointTestMode_;
    bool           bEmbeddedDispatcher_;
    bool           bPausedAtStartup_;
    bool           bEnableRandomBreakdowns_;
    tools::Path    strCheckPointNameTestMode_;
    int            randomSeed_;
    // non-conforming vector< bool > is so awesome
    std::vector< bool > randomGaussian_;
    std::vector< double > randomDeviation_;
    std::vector< double > randomMean_;
    unsigned int   setpause_;
    unsigned int   setstepmul_;
    tools::Path    integrationDir_;
    tools::Path    pathfindDir_;
    std::string    pathfindFilter_;
    bool           bTestCommands_;
    //@}
};

#endif // __MIL_Config_h_
