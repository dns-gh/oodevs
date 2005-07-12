// *****************************************************************************
//
// $Created: NLD 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_MedicalPriorities.h $
// $Author: Nld $
// $Modtime: 14/01/05 15:58 $
// $Revision: 1 $
// $Workfile: MOS_MTH_MedicalPriorities.h $
//
// *****************************************************************************

#ifndef __MOS_MTH_MedicalPriorities_h_
#define __MOS_MTH_MedicalPriorities_h_

#include "MOS_Types.h"
#include "MOS_MissionTool_ABC.h"

#include "MOS_ASN_Types.h"

class QListViewItem;
class QListView;
class QPopupMenu;


// =============================================================================
// Created: NLD 2003-12-03
// =============================================================================
class MOS_MTH_MedicalPriorities : public QWidget, public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MTH_MedicalPriorities )

public:
             MOS_MTH_MedicalPriorities( ASN1T_SantePriorites& asnMedicalPriorities, QWidget* pParent, const std::string& sName );
    virtual ~MOS_MTH_MedicalPriorities();

    //! @name Init
    //@{
    virtual void Initialize();
    virtual void Terminate ();
    //@}

    //! @name Tools
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission );
    virtual void SetWidgetTab( QWidget* pWidget );
    //@}

    //! @name Agent
    //@{
    virtual void SetAgent( MOS_Agent* pAgent );
    //@}

    //! @name DIN
    //@{
    virtual void WriteMsg               ( std::string& sParam );
    virtual void FillRandomParameters   ();
    virtual void ReloadParameters       ( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters       ( MT_OutputArchive_ABC& archive );
    //@}

    //! @name Notify
    //@{
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}

private slots:
    //! @name Tlos
    //@{
    void OnContextMenu( QListViewItem* pItem, const QPoint& pos, int );
    //@}

private:
    ASN1T_SantePriorites* pAsnMedicalPriorities_;

    QListView*  pTypeListView_;
    QPopupMenu* pTypesMenu_;
};

#include "MOS_MTH_MedicalPriorities.inl"

#endif // __MOS_MTH_MedicalPriorities_h_
