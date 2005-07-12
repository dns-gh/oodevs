//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_PointListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_Meteo_PointListView.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_PointListView_h_
#define __MOS_Meteo_PointListView_h_

#include "MOS_PointListView.h"

//*****************************************************************************
// Created: AGN 03-08-07
//*****************************************************************************
class MOS_Meteo_PointListView
: public MOS_PointListView
{
    MT_COPYNOTALLOWED( MOS_Meteo_PointListView )

public:
    explicit MOS_Meteo_PointListView( QWidget* pParent );
    virtual ~MOS_Meteo_PointListView();

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Draw                 ();
    virtual bool IsActive             ( bool bMove );
    virtual bool RMBPressEvent        ( const MT_Vector2D& vPos );
    virtual bool LMBPressEvent        ( const MT_Vector2D& vPos );
    virtual void MouseMoveEvent       ( const MT_Vector2D& vPos );
    virtual void RMBReleaseEvent      ( const MT_Vector2D& vPos );
    virtual void LMBReleaseEvent      ( const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name Points Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual MT_Vector2D            GetCenterPoint( MOS_PointListViewItem* pPoint );
    virtual MOS_PointListViewItem* IsInsidePoint    ( const MT_Vector2D& vPos ) const;
    void                   SetPosLine( MOS_PointListViewItem& point, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    //@}
    

    //-------------------------------------------------------------------------
    /** @name bouzes dues à l'héritage de MOS_MissionTool_ABC*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission );
    virtual void SetWidgetTab( QWidget* pWidget );
    virtual void SetAgent                 ( MOS_Agent* pAgent );
    virtual void WriteMsg     ( std::string& sParam );
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}

    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );

protected:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    virtual void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point ); // déconnecté
    //@}


private:
    MT_Vector2D      vOldPos_;
    bool             bMovingLine_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_PointListView.inl"
#endif


#endif // __MOS_Meteo_PointListView_h_