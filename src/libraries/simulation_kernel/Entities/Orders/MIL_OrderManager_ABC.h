//*****************************************************************************
//
// $Created: NLD 2003-01-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_OrderManager_ABC.h $
// $Author: Nld $
// $Modtime: 4/03/05 17:29 $
// $Revision: 4 $
// $Workfile: MIL_OrderManager_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_OrderManager_ABC_h_
#define __MIL_OrderManager_ABC_h_

#include "MIL.h"

class MIL_KnowledgeGroup;
class MIL_Mission_ABC;
class MIL_Fuseau;
class MIL_LimaFunction;
class MIL_MissionType_ABC;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_OrderManager_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderManager_ABC );

public:
    MIL_OrderManager_ABC();
    virtual ~MIL_OrderManager_ABC();

   // @name Main
    virtual void Update();
    //@}
 
    // @name Events 
    //@{
    void ReplaceMission( MIL_Mission_ABC* pMission = 0 ); // asynchronous
    //@}

    // @name Accessors 
    //@{
          MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function ) const;
          MIL_LimaOrder* FindLima             ( uint nID ) const;
          MIL_LimaOrder* FindNextScheduledLima() const;
    const T_LimaVector&  GetLimas             () const;
    const MT_Vector2D&   GetDirDanger         () const;
    const std::string&   GetMissionName       () const;
    //@}

    //! @name Missions accessors
    //@{
          bool                 IsNewMissionStarted  () const;
    const MIL_MissionType_ABC* GetCurrentMissionType() const;
    //@}

protected:
    //! @name Accessors
    //@{
    MIL_Mission_ABC* GetCurrentMission() const;
    //@}

    //! @name Operations
    //@{
    virtual void StopAllMissions(); // synchronous
    //@}

private:
    MIL_Mission_ABC* pMission_;
    MIL_Mission_ABC* pNextMission_;
    bool             bNewMissionStarted_;
};

#include "MIL_OrderManager_ABC.inl"

#endif // __MIL_OrderManager_ABC_h_
