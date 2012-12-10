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

#include "MIL_Random.h"
#include "tools/SessionConfig.h"
#include <boost/optional.hpp>
#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )
#include <map>

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
             MIL_Config( tools::RealFileLoaderObserver_ABC& observer );
    virtual ~MIL_Config();
    //@}

    //! @name Accessors
    //@{
    bool               CheckAutomateComposition    () const;
    bool               UseDecDebug                 () const;
    bool               UsePathDebug                () const;
    bool               IsDecisionalProfilingEnabled() const;
    bool               IsHookProfilingEnabled      () const;
    bool               IsCommandProfilingEnabled   () const;
    bool               IsDataTestMode              () const;
    bool               IsTestMode                  () const;
    bool               IsSaveCheckpointTestMode    () const;
    bool               IsDeleteCheckpointTestMode  () const;
    bool               IsDispatcherEmbedded        () const;
    unsigned short     GetNetworkPort              () const;
    unsigned long      GetNetworkTimeout           () const;
    bool               IsThreadedNetwork           () const;
    bool               IsLegacy                    () const;

    bool               UseCheckPointCRC            () const;
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
    const std::string& GetCheckpointNameTestMode   () const;
    const std::string& GetIntegrationDir           () const;

    int                GetRandomSeed               () const;
    const bool*        GetRandomGaussian           () const;
    const double*      GetRandomDeviation          () const;
    const double*      GetRandomMean               () const;
    unsigned int       GetGarbageCollectorPause    () const;
    unsigned int       GetGarbageCollectorStepMul  () const;
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );

    std::string BuildCheckpointChildFile( const std::string& file, std::string name = "" ) const;

    virtual std::string GetOrbatFile() const;
    //@}

    //! @name CheckPoints
    //@{
    void AddFileToCRC( const std::string& fileName );
    boost::crc_32_type::value_type serialize( const std::string& strFileName ) const;
    //@}

private:
    //! @name Assignment operator
    //@{
    MIL_Config( const MIL_Config& );
    MIL_Config& operator=( const MIL_Config& );
    //@}

    //! @name Helpers
    //@{
    void ReadSessionFile            ( const std::string& file );
    void ReadSessionXml             ( xml::xistream& xis );
    void ReadCheckPointConfiguration( xml::xistream& xis );
    void ReadDebugConfiguration     ( xml::xistream& xis );
    void ConfigureRandom            ( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< const std::string, boost::crc_32_type::value_type > T_CRCMap;
    typedef T_CRCMap::const_iterator                                      CIT_CRCMap;
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
    unsigned short networkPort_;
    unsigned long  networkTimeOut_;
    bool           bCheckPointOrbat_;
    bool           bUseCheckPointCRC_;
    bool           bCheckAutomateComposition_;
    bool           bUseDecDebug_;
    bool           bUsePathDebug_;
    bool           bUseNetworkLogger_;
    bool           bDecisionalProfilingEnabled_;
    bool           bHookProfilingEnabled_;
    bool           bCommandProfilingEnabled_;
    bool           bDataTestMode_;
    bool           bTestMode_;
    bool           bSaveCheckpointTestMode_;
    bool           bDeleteCheckpointTestMode_;
    bool           bEmbeddedDispatcher_;
    bool           bPausedAtStartup_;
    bool           bLegacy_;
    std::string    strCheckPointNameTestMode_;
    T_CRCMap       CRCMap_;
    int            randomSeed_;
    bool           randomGaussian_[ MIL_Random::eContextsNbr ];
    double         randomDeviation_[ MIL_Random::eContextsNbr ];
    double         randomMean_[ MIL_Random::eContextsNbr ];
    unsigned int   setpause_;
    unsigned int   setstepmul_;
    std::string    integrationDir_;
    //@}
};

#   include "MIL_Config.inl"

#endif // __MIL_Config_h_
