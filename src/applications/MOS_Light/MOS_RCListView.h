//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_RCListView.h $
//
//*****************************************************************************

#ifndef __MOS_RCListView_h_
#define __MOS_RCListView_h_

#include "MOS_Types.h"

#include <QListView.h>
#include "MOS_ListViewItem_ABC.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_LineManager.h"
#include "MOS_Agent.h"

class QWidget;
class QPopupMenu;
class MOS_RCListViewItem;
class MOS_RCCheckListItem;
class MOS_PointListViewItem;

class QListViewItem;
class MOS_RC;

    
//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_RCListView : public QListView
                       , public MOS_GLMouse_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_RCListView );

private: 
    typedef std::vector< QListViewItem* >           T_RCPtrVector;
    typedef const T_RCPtrVector                   CT_RCPtrVector;
    typedef T_RCPtrVector::iterator               IT_RCPtrVector;
    typedef T_RCPtrVector::const_iterator         CIT_RCPtrVector;
    typedef T_RCPtrVector::reverse_iterator       RIT_RCPtrVector;

public:


    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_RCListView( QWidget* pParent, E_ListViewType nType );
    virtual ~MOS_RCListView();
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
    QListViewItem*         CreateRC                 ( MOS_RC& RC );
    void                   DeleteRC                 ( QListViewItem& RC );
    void                   RegisterRC               ( QListViewItem& RC );
    void                   UnregisterRC             ( QListViewItem& RC );
    MOS_RCListView&        GetRCListView            () const;
    QListViewItem*         CreateRCFromInterface    ();
    void                   DeleteRCFromInterface    ( QListViewItem& RC );
    MOS_PointListViewItem* CreatePointFromInterface ( QListViewItem& RC, MOS_PointListViewItem* pPoint = 0, MT_Vector2D* pPos = 0 );
    void                   DeletePointFromInterface ( QListViewItem& RC, MOS_PointListViewItem& point );
    void                   SetPosPoint              ( QListViewItem& RC, MOS_PointListViewItem& point, const MT_Vector2D& vPos );
    void                   SetPosRC                 ( QListViewItem& RC, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    void                   LocOnRC                  ( QListViewItem* pPoint, bool bSelectList );
    E_ListViewType         GetType                  () const;
    void                   SetAgent                 ( MOS_Agent* pAgent );
    MOS_Agent*             GetAgent                 ();
    void                   SetOldPos                ( const MT_Vector2D& vPos );
    MT_Vector2D&           GetOldPos                ();
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw();
    bool IsActive( bool bMove );
    bool RMBPressEvent( const MT_Vector2D& vPos );
    bool LMBPressEvent( const MT_Vector2D& vPos );
    void MouseMoveEvent( const MT_Vector2D& vPos );
    void RMBReleaseEvent( const MT_Vector2D& vPos );
    void LMBReleaseEvent( const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name Events*/
    //-------------------------------------------------------------------------
    //@{
    void mousePressEvent( QMouseEvent* pMouseEvent );
    void keyPressEvent ( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
    void showEvent ( QShowEvent * );
    //@}


protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point );
    void SlotItemPressed         ( QListViewItem* pItem );
    void SlotSetShowAll          ( bool );
    //@}

private:
    bool                    bMovingPoint_;
    bool                    bMovingLine_;
    bool                    bCtrl_;
    bool                    bShift_;
    bool                    bDrawAll_;

    QListViewItem*          pSelectedRC_;
    MT_Vector2D             vPosMovingLine_;

    T_RCPtrVector         RCVector_;
    QPopupMenu*             pMenu_;
    QPopupMenu*             pMenuRC_;
    QPopupMenu*             pMenuPoint_;

    E_ListViewType          nType_;
    bool                    bActive_;

    MT_Vector2D             vOldPos_;
    MOS_Agent*              pAgent_;

    std::string             sFileName_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_RCListView.inl"
#endif


#endif // __MOS_RCListView_h_