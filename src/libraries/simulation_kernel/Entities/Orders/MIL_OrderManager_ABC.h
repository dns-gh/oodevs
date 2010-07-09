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

#include "MIL_LimaOrder.h"

class MIL_KnowledgeGroup;
class MIL_Mission_ABC;
class MIL_Fuseau;
class MIL_LimaFunction;
class MIL_MissionType_ABC;
class MIL_IntelligenceOrdersVisitor_ABC;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_OrderManager_ABC : private boost::noncopyable
{
public:
             MIL_OrderManager_ABC();
    virtual ~MIL_OrderManager_ABC();

   // @name Main
    virtual void Update();
    //@}

    // @name Events
    //@{
    void ReplaceMission( boost::shared_ptr< MIL_Mission_ABC > pMission  ); // asynchronous
    void CancelMission(); // asynchronous
    virtual void StopAllMissions(); // synchronous
    //@}

    // @name Accessors
    //@{
          virtual MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function ) const;
          virtual MIL_LimaOrder* FindLima             ( unsigned int nID ) const;
          virtual MIL_LimaOrder* FindNextScheduledLima() const;
    const T_LimaVector&  GetLimas             () const;
    const MT_Vector2D&   GetDirDanger         () const;
    const std::string&   GetMissionName       () const;
    void                 Accept               ( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const;
    //@}

    //! @name Missions accessors
    //@{
          bool                 IsNewMissionStarted  () const;
    const MIL_MissionType_ABC* GetCurrentMissionType() const;
    //@}


protected:
    //! @name Accessors
    //@{
    boost::shared_ptr< MIL_Mission_ABC > GetCurrentMission() const;
    //@}

private:
    boost::shared_ptr< MIL_Mission_ABC > pMission_;
    boost::shared_ptr< MIL_Mission_ABC > pNextMission_;
    bool             bNewMissionStarted_;
};

#endif // __MIL_OrderManager_ABC_h_
