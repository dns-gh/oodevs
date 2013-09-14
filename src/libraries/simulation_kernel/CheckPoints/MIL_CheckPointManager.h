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
#include <queue>
#include <tools/Path.h>

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
    void Update();

    static void LoadCheckPoint( const MIL_Config& config, const ObjectTypeResolver_ABC& resolver );
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
    void RotateCheckPoints       ( const tools::Path& newName );
    bool SaveCheckPoint          ( const tools::Path& name, const tools::Path& userName = "" );
    bool SaveOrbatCheckPoint     ( const tools::Path& name );
    bool SaveFullCheckPoint      ( const tools::Path& name, const tools::Path& userName = "" );
    tools::Path BuildCheckPointName() const;

    void UpdateNextCheckPointTick();
    //@}

private:
    //! @name types
    //@{
    typedef std::queue< tools::Path > T_CheckPointsQueue;
    //@}

private:
    unsigned int         nMaxCheckPointNbr_;
    unsigned int         nCheckPointsFrequency_;
    unsigned int         nLastCheckPointTick_;
    unsigned int         nNextCheckPointTick_;
    tools::Path          checkpointName_;
    T_CheckPointsQueue   currentCheckPoints_;
};

#endif // __MIL_CheckPointManager_h_
