//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitMission.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 14 $
// $Workfile: MOS_LimitMission.h $
//
//*****************************************************************************

#ifndef __MOS_LimitMission_h_
#define __MOS_LimitMission_h_

#include "MOS_Types.h"
#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>

class MOS_LimitListView;
class QWidget;
class MOS_Agent;
class MOS_Mission_ABC;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimitMission : public QFrame
                       , public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimitMission );

public:
    explicit MOS_LimitMission( ASN1T_OID& asnLeftLimitID, ASN1T_OID& asnRightLimitID, QWidget* pParent = 0 );
    virtual ~MOS_LimitMission();

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool IsActive( bool bMove );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_LimitListView&      GetLimitListView() const;
    void                    SetAgent        ( MOS_Agent* pAgent );
    void                    SetMission      ( MOS_Mission_ABC* pMission );
    void                    SetWidgetTab    ( QWidget* pWidget );
    void                    NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg     ( std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

private:
    ASN1T_OID&           asnLeftLimitID_;
    ASN1T_OID&           asnRightLimitID_;
    MOS_LimitListView*   pLimitListView_;
    MOS_Mission_ABC*     pMission_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_LimitMission.inl"
#endif


#endif // __MOS_LimitMission_h_