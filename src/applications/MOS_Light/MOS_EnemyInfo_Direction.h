//*****************************************************************************
//
// $Created: AGN 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Direction.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_EnemyInfo_Direction.h $
//
//*****************************************************************************

#ifndef __MOS_EnemyInfo_Direction_h_
#define __MOS_EnemyInfo_Direction_h_

#include "MOS_PointListView.h"

//*****************************************************************************
// Created: AGN 03-05-21
//*****************************************************************************
class MOS_EnemyInfo_Direction
: public MOS_PointListView
{
    MT_COPYNOTALLOWED( MOS_EnemyInfo_Direction )
public:
    MOS_EnemyInfo_Direction( QWidget* pParent, const std::string& strName );
    virtual ~MOS_EnemyInfo_Direction();

    virtual void Initialize();
    virtual void Terminate();
    
    virtual void Draw                 ();
    virtual bool IsActive             ( bool bMove );
    virtual bool RMBPressEvent        ( const MT_Vector2D& vPos );
    virtual bool LMBPressEvent        ( const MT_Vector2D& vPos );
    virtual void MouseMoveEvent       ( const MT_Vector2D& vPos );
    virtual void RMBReleaseEvent      ( const MT_Vector2D& vPos );
    virtual void LMBReleaseEvent      ( const MT_Vector2D& vPos );

    virtual MT_Vector2D            GetCenterPoint( MOS_PointListViewItem* pPoint );
    virtual MOS_PointListViewItem* IsInsidePoint    ( const MT_Vector2D& vPos ) const;


    // NOT USED
    virtual void SetMission( MOS_Mission_ABC* pMission );
    virtual void SetWidgetTab( QWidget* pWidget );
    virtual void SetAgent( MOS_Agent* pAgent );
    virtual void WriteMsg( std::string& sParam );
    virtual void FillRandomParameters();
    virtual void ReloadParameters( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters( MT_OutputArchive_ABC& archive );
     

    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );


protected slots:
    virtual void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point );

private:
    void                   SetPosLine( const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    
private:
    bool                    bMovingLine_;
    MT_Vector2D             vOldPos_;
    std::string             sName_;
    bool                    bFirstWidget_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_EnemyInfo_Direction.inl"
#endif


#endif // __MOS_EnemyInfo_Direction_h_