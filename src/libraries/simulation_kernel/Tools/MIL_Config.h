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

#include "MIL.h"

#include "MIL_Random.h"
#include "tools/SessionConfig.h"

#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: NLD 2003-11-21
// =============================================================================
class MIL_Config : public virtual tools::SessionConfig
{

public:
     MIL_Config();
    virtual ~MIL_Config();

    //! @name Accessors
    //@{
    bool               UseOnlyDIAArchive         () const;
    bool               CheckAutomateComposition  () const;
    bool               UseDecDebug               () const;
    bool               UsePathDebug              () const;
    bool               IsProfilingEnabled        () const;
    bool               IsDataTestMode            () const;
    bool               IsTestMode                () const;
    bool               IsSaveCheckpointTestMode  () const;
    bool               IsDeleteCheckpointTestMode() const;
    bool               IsFrozenMode              () const;
    bool               IsDispatcherEmbedded      () const;
    unsigned short     GetNetworkPort            () const;
    bool               IsThreadedNetwork         () const;

    bool               UseCheckPointCRC          () const;
    bool               UseDiaDebugger            () const;
    unsigned short     GetDiaDebuggerPort        () const;
    bool               UseNetworkLogger          () const;
    unsigned short     GetNetworkLoggerPort      () const;
    unsigned int       GetCheckPointsFrequency   () const;
    unsigned int       GetCheckPointsKept        () const;
    unsigned int       GetTimeStep               () const;
    unsigned int       GetTimeFactor             () const;
    unsigned int       GetEndTick                () const;
    unsigned int       GetPathFinderThreads      () const;
    bool               IsHLAEnabled              () const;
    const std::string& GetHLAFederation          () const;
    const std::string& GetHLAFederate            () const;
    const std::string& GetCheckpointNameTestMode () const;

    int                GetRandomSeed             () const;
    const bool*        GetRandomGaussian         () const;
    const int*         GetRandomDeviation        () const;
    const int*         GetRandomMean             () const;
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );

    std::string BuildCheckpointChildFile( const std::string& file, std::string name = "" ) const;

    bool IsPopulationEnabled() const;
    std::string GetPopulationDir() const;

    virtual std::string GetOrbatFile() const;
    virtual std::string GetUrbanFile() const;
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
    void ReadCheckPointConfiguration( xml::xistream& xis );
    void ReadDebugConfiguration     ( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< const std::string, boost::crc_32_type::value_type > T_CRCMap;
    typedef T_CRCMap::const_iterator                                      CIT_CRCMap;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int               checkPointsKept_;
    unsigned int               checkPointsFrequency_;
    unsigned int               timeStep_;
    unsigned int               timeFactor_;
    unsigned int               endTick_;
    unsigned int               pathFinderThreads_;
    unsigned short             diaDebuggerPort_;
    unsigned short             networkLoggerPort_;
    unsigned short             networkPort_;
    bool                       bCheckPointOrbat_;
    bool                       bUseCheckPointCRC_;
    bool                       bUseOnlyDIAArchive_;
    bool                       bCheckAutomateComposition_;
    bool                       bUseDecDebug_;
    bool                       bUsePathDebug_;
    bool                       bUseDiaDebugger_;
    bool                       bUseNetworkLogger_;
    bool                       bProfilingEnabled_;
    bool                       bDataTestMode_;
    bool                       bTestMode_;
    bool                       bSaveCheckpointTestMode_;
    bool                       bDeleteCheckpointTestMode_;
    bool                       bFrozenMode_;
    bool                       bEmbeddedDispatcher_;
    bool                       bHLAEnabled_;
    std::string                hlaFederation_;
    std::string                hlaFederate_;
    std::string                strCheckPointNameTestMode_;
    T_CRCMap                   CRCMap_;
    int                        randomSeed_;
    bool                       randomGaussian_[ MIL_Random::eContextsNbr ];
    int                        randomDeviation_[ MIL_Random::eContextsNbr ];
    int                        randomMean_[ MIL_Random::eContextsNbr ];
    //@}
};

#   include "MIL_Config.inl"

#endif // __MIL_Config_h_
