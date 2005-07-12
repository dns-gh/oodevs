//*****************************************************************************
//
// $Created: FBD 03-01-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MissionTool_ABC.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_MissionTool_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_MissionTool_ABC_h_
#define __MOS_MissionTool_ABC_h_

#include "MOS_Types.h"

class MOS_Mission_ABC;
class QWidget;
class MOS_Agent;
class QVBoxLayout;
class QLabel;


//*****************************************************************************
// Created: FBD 03-01-10
//*****************************************************************************
class MOS_MissionTool_ABC
{
public:
    explicit MOS_MissionTool_ABC();
    virtual ~MOS_MissionTool_ABC();

public:

    //-------------------------------------------------------------------------
    /** @name Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission ) = 0;
    virtual void SetWidgetTab( QWidget* pWidget ) = 0;
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetAgent( MOS_Agent* pAgent ) = 0;
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    virtual void WriteMsg               ( std::string& sParam ) = 0;
    virtual void FillRandomParameters   () = 0;
    virtual void ReloadParameters       ( MT_InputArchive_ABC& archive ) = 0;
    virtual void ResaveParameters       ( MT_OutputArchive_ABC& archive ) = 0;
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos ) = 0;
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize() = 0;
    virtual void Terminate() = 0;
    //@} 

protected:
    //! @name Tools
    //@{
    QLabel* CreateTitle( QWidget* pParent, const std::string& sName );
    //@}

};

#ifdef MOS_USE_INLINE
#	include "MOS_MissionTool_ABC.inl"
#endif


#endif // __MOS_MissionTool_ABC_h_