//*****************************************************************************
//
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_ListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_MTH_ListLocalisation_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_ListLocalisation_ListView_h_
#define __MOS_MTH_ListLocalisation_ListView_h_

#include "MOS_Types.h"

#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

#include <qlistview.h>


class MOS_MTH_ListLocalisation_Editor;
class QVBoxLayout;

//*****************************************************************************
// Created: AGN 03-08-08
//*****************************************************************************
class MOS_MTH_ListLocalisation_ListView
: public QListView
, public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MTH_ListLocalisation_ListView );

public:
    MOS_MTH_ListLocalisation_ListView( ASN1T_ListLocalisation& asnListLocalisation, QWidget* pParent, const std::string& strName );
    MOS_MTH_ListLocalisation_ListView( ASN1T_ListPolygon& asnListPolygon, QWidget* pParent, const std::string& strName );
    virtual ~MOS_MTH_ListLocalisation_ListView();

    //-------------------------------------------------------------------------
    /** @name Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission ) ;
    virtual void SetWidgetTab( QWidget* pWidget ) ;
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetAgent( MOS_Agent* pAgent ) ;
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    virtual void WriteMsg( std::string& sParam ) ;
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos ) ;
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize() ;
    virtual void Terminate() ;
    //@}
    
private slots:
    void OnContextMenu( QListViewItem* pItem, const QPoint& pos, int );

private:
    enum E_ContextMenu_Item
    {
        eContextMenu_AddLocalisation,
        eContextMenu_EditLocalisation,
        eContextMenu_RemoveLocalisation
    };

private:
    MOS_Agent* pAgent_;
    MOS_MTH_ListLocalisation_Editor* pEditor_;

    ASN1T_ListLocalisation& asnListLocalisation_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_ListLocalisation_ListView.inl"
#endif


#endif // __MOS_MTH_ListLocalisation_ListView_h_