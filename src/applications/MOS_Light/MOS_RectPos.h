//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RectPos.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_RectPos.h $
//
//*****************************************************************************

#ifndef __MOS_RectPos_h_
#define __MOS_RectPos_h_

#include "MOS_Types.h"
#include "MOS_PointListView.h"

class QWidget;
class QPopupMenu;
class QListViewItem;
class MOS_Agent;
class MOS_Mission_ABC;
class QWidget;

   
//*****************************************************************************
// Created: FBD 03-01-13
//*****************************************************************************
class MOS_RectPos : public MOS_PointListView
{
    MT_COPYNOTALLOWED( MOS_RectPos );

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_RectPos( QWidget* pParent, const std::string& sName );
    virtual ~MOS_RectPos();
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    void SetAgent     ( MOS_Agent* pAgent );
    void SetMission   ( MOS_Mission_ABC* pMission );
    void SetWidgetTab ( QWidget* pWidget );
    void SetFirstWidget( bool bFirstWidget );
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw            ();
    bool IsActive        ( bool bMove );
    bool RMBPressEvent   ( const MT_Vector2D& vPos );
    bool LMBPressEvent   ( const MT_Vector2D& vPos );
    void MouseMoveEvent  ( const MT_Vector2D& vPos );
    void RMBReleaseEvent ( const MT_Vector2D& vPos );
    void LMBReleaseEvent ( const MT_Vector2D& vPos );
    //@}

     //-------------------------------------------------------------------------
    /** @name Points Tools*/
    //-------------------------------------------------------------------------
    //@{
    MT_Vector2D            GetCenterPoint( MOS_PointListViewItem* pPoint );
    MOS_PointListViewItem* IsInsidePoint( const MT_Vector2D& vPos ) const;
    void                   SetPosLine( MOS_PointListViewItem& point, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    void                   UpdatePosRect( const MT_Vector2D& vPos );
    //@}


    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg     ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point );
    //@}

private:

    MOS_Agent*       pAgent_;
    MT_Vector2D      vOldPos_;
    MOS_Mission_ABC* pMission_;
    QWidget*         pWidgetTab_;
    bool             bMovingLine_;
    bool             bFirstWidget_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_RectPos.inl"
#endif


#endif // __MOS_RectPos_h_