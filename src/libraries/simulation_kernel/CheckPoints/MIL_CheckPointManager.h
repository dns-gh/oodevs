// *****************************************************************************
//
// $Created: NLD 2003-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/checkpoints/MIL_CheckPointManager.h $
// $Author: Age $
// $Modtime: 20/05/05 15:16 $
// $Revision: 6 $
// $Workfile: MIL_CheckPointManager.h $
//
// *****************************************************************************

#ifndef __MIL_CheckPointManager_h_
#define __MIL_CheckPointManager_h_

#include "MIL.h"

#include "game_asn/Asn.h"

#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )

#include <queue>

class MIL_Config;

// =============================================================================
// Created: NLD 2003-08-05
// =============================================================================
class MIL_CheckPointManager
{
    MT_COPYNOTALLOWED( MIL_CheckPointManager )
    
public:
              MIL_CheckPointManager();
     explicit MIL_CheckPointManager( const MIL_Config& config );
    ~MIL_CheckPointManager();

    //! @name Main
    //@{
    void Update        ();
    void LoadCheckPoint( const MIL_Config& config );
    //@}

    //! @name Accessors
    //@{
    uint GetCheckPointFrequency() const;
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgCheckPointSaveNow     ( const ASN1T_MsgCtrlCheckPointSaveNow&      asnMsg );
    void OnReceiveMsgCheckPointSetFrequency( const ASN1T_MsgCtrlCheckPointSetFrequency& asnMsg );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

private:   
    //! @name Tools
    //@{
    void RotateCheckPoints       ( const std::string& newName );
    bool SaveCheckPoint          ( const std::string& name, const std::string userName = "" );
    bool SaveOrbatCheckPoint     ( const std::string& name );
    bool SaveFullCheckPoint      ( const std::string& name, const std::string userName = "" );

    void UpdateNextCheckPointTick();
    //@}
    
    //! @name Tools
    //@{
    static void                           CreateMetaData     ( const std::string& strPath, const std::string& strName, const boost::crc_32_type::value_type&, const boost::crc_32_type::value_type& );
    static boost::crc_32_type::value_type CreateData         ( const std::string& strFileName );
    static const std::string              BuildCheckPointName();

    static void                           CheckCRC           ( const MIL_Config& config );
    static void                           CheckFilesCRC      ( const MIL_Config& config );
    //@}

private:
    //! @name types
    //@{
    typedef std::queue< std::string > T_CheckPointsQueue;
    //@}
    
private:
    const uint                 nMaxCheckPointNbr_;
          uint                 nCheckPointsFrequency_; 
          uint                 nLastCheckPointTick_;
          uint                 nNextCheckPointTick_;
          T_CheckPointsQueue   currentCheckPoints_;
};

#include "MIL_CheckPointManager.inl"

#endif // __MIL_CheckPointManager_h_
