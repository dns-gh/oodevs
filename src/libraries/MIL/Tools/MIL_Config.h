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

#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )

// =============================================================================
// Created: NLD 2003-11-21
// =============================================================================
class MIL_Config
{
public:
     MIL_Config();
    ~MIL_Config();

    //! @name Accessors
    //@{
          uint         GetExerciceID              () const;
          bool         UseDIAArchive              () const;
          uint         GetNbrPathFindThreads      () const;
          bool         ForceODBAutomateComposition() const;
          bool         UseDecDebug                () const;
          bool         UsePathDebug               () const;
          bool         IsProfilingEnabled         () const;
    const std::string& GetCheckPointFileName      () const;
    const std::string& GetConfigFileName          () const;
    
          bool         UseCheckPoint    () const;
          bool         UseCheckPointCRC () const;
          bool         UseDiaDebugServer() const;
    //@}

    //! @name CheckPoints
    //@{
    void AddFileToCRC( const std::string& fileName );

    boost::crc_32_type::value_type serialize( const std::string& strFileName ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetExerciceID                 ( uint nExercideID );
    void SetUseDIAArchive              ( bool bUseDIAArchive );
    void SetNbrPathFindThreads         ( uint nNbr );
    void SetForceODBAutomateComposition( bool bForce );
    void SetCheckPointFileName         ( const std::string& strName );
    void SetConfigFileName             ( const std::string& strName );
    void SetUseDecDebug                ( bool bUse );
    void SetUsePathDebug               ( bool bUse );
    void SetUseDiaDebugServer          ( bool bUse );
    void SetProfilingEnabled           ( bool bEnabled );
    void SetUseCheckPointCRC           ( bool bUse );
    //@}

private:
    MIL_Config& operator = ( const MIL_Config& );
    
private:
    typedef std::map< const std::string, boost::crc_32_type::value_type > T_CRCMap;
    typedef T_CRCMap::const_iterator                                      CIT_CRCMap;

private:
    const std::string   strRootDirectory_;
    std::string         strConfFile_;
    std::string         strCheckPointFileName_;
    uint                nExerciceID_;
    uint                nNbrPathFindThreads_;
    bool                bUseCheckPointCRC_;
    bool                bUseDIAArchive_;
    bool                bForceODBAutomateComposition_;
    bool                bUseDecDebug_;
    bool                bUseDiaDebugServer_;
    bool                bProfilingEnabled_;
    bool                bUsePathDebug_;
    T_CRCMap            CRCMap_;
};

#   include "MIL_Config.inl"
#endif // __MIL_Config_h_
