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
#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )
#include <queue>

namespace sword
{
    class ControlCheckPointSaveNow;
    class ControlCheckPointSetFrequency;
}

namespace xml
{
    class xostream;
}

class MIL_Config;

// =============================================================================
// Created: NLD 2003-08-05
// =============================================================================
class MIL_CheckPointManager : private boost::noncopyable
{
public:
              MIL_CheckPointManager();
     explicit MIL_CheckPointManager( const MIL_Config& config );
     virtual ~MIL_CheckPointManager();

    //! @name Main
    //@{
    void Update                 ();
    static void LoadCheckPoint  ( const MIL_Config& config );
    void SaveCheckPointDirectory( const std::string& directoryName, const std::string userName = "" );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetCheckPointFrequency() const;
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgCheckPointSaveNow     ( const sword::ControlCheckPointSaveNow&      msg );
    void OnReceiveMsgCheckPointSetFrequency( const sword::ControlCheckPointSetFrequency& msg );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
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
    unsigned int         nMaxCheckPointNbr_;
    unsigned int         nCheckPointsFrequency_;
    unsigned int         nLastCheckPointTick_;
    unsigned int         nNextCheckPointTick_;
    T_CheckPointsQueue   currentCheckPoints_;
};

#endif // __MIL_CheckPointManager_h_
