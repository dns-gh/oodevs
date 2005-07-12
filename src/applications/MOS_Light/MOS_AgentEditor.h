//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentEditor.h $
// $Author: Nld $
// $Modtime: 15/03/05 19:03 $
// $Revision: 14 $
// $Workfile: MOS_AgentEditor.h $
//
//*****************************************************************************

#ifndef __MOS_AgentEditor_h_
#define __MOS_AgentEditor_h_
 
#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>

class MOS_AgentListView;
class MOS_Agent;
class MOS_EnemyInfo_List;
class MOS_Value;
class MOS_Team;
class MOS_ODBListView;
class MOS_ChangeDiplomacyDialog;
class QListView;
class QCheckBox;
class QLineEdit;
class QWidget;
class QTabWidget;
class QPushButton;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_AgentEditor : public QWidget
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_AgentEditor );

public:
    explicit MOS_AgentEditor( QWidget* pParent = 0 );
    virtual ~MOS_AgentEditor();

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
    bool IsActive   ( bool bMove );
    bool MustBeDrawn( const MOS_Team& team ) const;
    //@}

    void Draw();

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_AgentListView& GetAgentListView() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    void NotifyEnableUnitVisionCones( bool bEnable );
    void NotifyEnableProfiling      ( bool bEnable );

    void NotifyDeleteAgent( MOS_Agent& agent );
    void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    void NotifyAgentMissionAck( ASN1T_EnumOrderErrorCode nErrorCode );
    //@}

public slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotChangeCheckPointFrequency();
    void SlotCheckPointSaveNow        ();
    void SlotChangeDiplomacy          ();
    //@}

private:
    MOS_AgentListView*          pAgentListView_;
    QTabWidget*                 pTabWidget_;
    QWidget*                    pSettingsWidget_;
    QWidget*                    pInfoEnemyWidget_;
    MOS_EnemyInfo_List*         pRensEni_;
    QWidget*                    pODBWidget_;
    MOS_ODBListView*            pOdbListView_;
    QListView*                  pDrawTeamList_;
    MOS_ChangeDiplomacyDialog*  pChangeDiplomacyDialog_;

    MOS_Value*   pCheckPointFrequency_;
    QPushButton* pButtonCheckPointSaveNow_;
    QLineEdit*   pCheckPointName_;
    
    QCheckBox*   pCheckBoxCone_;
    QCheckBox*   pCheckBoxProfiling_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_AgentEditor.inl"
#endif


#endif // __MOS_AgentEditor_h_