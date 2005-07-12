//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MagicPos.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 11 $
// $Workfile: MOS_MagicPos.h $
//
//*****************************************************************************

#ifndef __MOS_MagicPos_h_
#define __MOS_MagicPos_h_

#include "MOS_Types.h"
#include "MOS_PointListView.h"

#include "MOS_ASN_Types.h"

class QWidget;
class QPopupMenu;
class QListViewItem;
class MOS_Agent;
class MOS_AttrEditor;
class QWidget;
class MOS_Mission_ABC;
class QLabel;
   
//*****************************************************************************
// Created: FBD 03-01-13
//*****************************************************************************
class MOS_MagicPos
: public MOS_PointListView
{
    MT_COPYNOTALLOWED( MOS_MagicPos );

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    MOS_MagicPos( QWidget* pParent, const std::string& sName );
    ~MOS_MagicPos();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    void SetMission( MOS_Mission_ABC* pMission );
    void SetAttrEditor( MOS_AttrEditor* pAttrEditor );
    void SetWidgetTab( QWidget* pWidget );
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    void SetAgent       ( MOS_Agent* pAgent );
    bool IsModifiedAgent() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw           ();
    bool IsActive       ( bool bMove );
    bool RMBPressEvent  ( const MT_Vector2D& vPos );
    bool LMBPressEvent  ( const MT_Vector2D& vPos );
    void MouseMoveEvent ( const MT_Vector2D& vPos );
    void RMBReleaseEvent( const MT_Vector2D& vPos );
    void LMBReleaseEvent( const MT_Vector2D& vPos );
    //@}

     //-------------------------------------------------------------------------
    /** @name Points Tools*/
    //-------------------------------------------------------------------------
    //@{
    MT_Vector2D            GetCenterPoint( MOS_PointListViewItem* pPoint );
    MOS_PointListViewItem* IsInsidePoint( const MT_Vector2D& vPos ) const;
    //@}


    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

    void Show( bool bShow );

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

    MOS_Agent*              pAgent_;
    bool                    bModifiedAgent_;
    MT_Vector2D             vOldPos_;
    MOS_AttrEditor*         pAttrEditor_;
    QWidget*                pWidgetTab_;
    std::string             sName_;
    QLabel*                 pLabel_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MagicPos.inl"
#endif


#endif // __MOS_MagicPos_h_