//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_DirectionPos.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_MTH_DirectionPos.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_DirectionPos_h_
#define __MOS_MTH_DirectionPos_h_

#include "MOS_Types.h"
#include "MOS_PointListView.h"
#include "MOS_ASN_Types.h"

class QWidget;
class QPopupMenu;
class QListViewItem;
class MOS_Agent;
class MOS_Mission_ABC;
class QWidget;

    
//*****************************************************************************
// Created: FBD 03-01-13
//*****************************************************************************
class MOS_MTH_DirectionPos : public MOS_PointListView
{
    MT_COPYNOTALLOWED( MOS_MTH_DirectionPos );

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_MTH_DirectionPos( ASN1T_Direction& asnDirection, QWidget* pParent, const std::string& sName, bool bFollow = true );
    virtual ~MOS_MTH_DirectionPos();
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
    void SetMission( MOS_Mission_ABC* pMission );
    void SetWidgetTab( QWidget* pWidget );
    void SetFirstWidget( bool bFirstWidget );
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    void SetAgent                 ( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw                 ();
    bool IsActive             ( bool bMove );
    bool RMBPressEvent        ( const MT_Vector2D& vPos );
    bool LMBPressEvent        ( const MT_Vector2D& vPos );
    void MouseMoveEvent       ( const MT_Vector2D& vPos );
    void RMBReleaseEvent      ( const MT_Vector2D& vPos );
    void LMBReleaseEvent      ( const MT_Vector2D& vPos );
    //@}

     //-------------------------------------------------------------------------
    /** @name Points Tools*/
    //-------------------------------------------------------------------------
    //@{
    MT_Vector2D            GetCenterPoint( MOS_PointListViewItem* pPoint );
    MOS_PointListViewItem* IsInsidePoint( const MT_Vector2D& vPos ) const;
    void                   SetPosLine( MOS_PointListViewItem& point, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    //@}

     //-------------------------------------------------------------------------
    /** @name Events*/
    //-------------------------------------------------------------------------
    //@{
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg               ( std::string& sParam );
    void FillRandomParameters   ();
    void ReloadParameters       ( MT_InputArchive_ABC& archive );
    void ResaveParameters       ( MT_OutputArchive_ABC& archive );
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
	ASN1T_Direction&		asnDirection_;

    std::string             sName_;
    bool                    bMovingLine_;
    bool                    bFollow_;
    MOS_Agent*              pAgent_;
    MT_Vector2D             vOldPos_;
    MOS_Mission_ABC*        pMission_;
    QWidget*                pWidgetTab_;
    bool                    bFirstWidget_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_DirectionPos.inl"
#endif


#endif // __MOS_MTH_DirectionPos_h_