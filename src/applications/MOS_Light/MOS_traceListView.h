//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_traceListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 8 $
// $Workfile: MOS_traceListView.h $
//
//*****************************************************************************

#ifndef __MOS_TraceListView_h_
#define __MOS_TraceListView_h_

#include "MOS_Types.h"

#include <QListView.h>
#include "MOS_ListViewItem_ABC.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_LineManager.h"
#include "MOS_Agent.h"

class QWidget;
class QPopupMenu;
class MOS_TraceListViewItem;
class MOS_TraceCheckListItem;
class MOS_PointListViewItem;

class QListViewItem;
class MOS_Trace;

    
//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_TraceListView : public QListView
                       , public MOS_GLMouse_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_TraceListView );

private: 
    typedef std::vector< QListViewItem* >           T_TracePtrVector;
    typedef const T_TracePtrVector                   CT_TracePtrVector;
    typedef T_TracePtrVector::iterator               IT_TracePtrVector;
    typedef T_TracePtrVector::const_iterator         CIT_TracePtrVector;
    typedef T_TracePtrVector::reverse_iterator       RIT_TracePtrVector;

public:


    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_TraceListView( QWidget* pParent, E_ListViewType nType );
    virtual ~MOS_TraceListView();
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
    QListViewItem*         CreateTrace                 ( MOS_Trace& Trace );
    void                   DeleteTrace                 ( QListViewItem& Trace );
    void                   RegisterTrace               ( QListViewItem& Trace );
    void                   UnregisterTrace             ( QListViewItem& Trace );
    MOS_TraceListView&        GetTraceListView            () const;
    QListViewItem*         CreateTraceFromInterface    ();
    void                   DeleteTraceFromInterface    ( QListViewItem& Trace );
    MOS_PointListViewItem* CreatePointFromInterface ( QListViewItem& Trace, MOS_PointListViewItem* pPoint = 0, MT_Vector2D* pPos = 0 );
    void                   DeletePointFromInterface ( QListViewItem& Trace, MOS_PointListViewItem& point );
    void                   SetPosPoint              ( QListViewItem& Trace, MOS_PointListViewItem& point, const MT_Vector2D& vPos );
    void                   SetPosTrace                 ( QListViewItem& Trace, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    void                   LocOnTrace                  ( QListViewItem* pPoint, bool bSelectList );
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
    void SlotItemPressed         ( QListViewItem* pItem );
    void SlotSetShowAll          ( bool );
    //@}

private:
    bool                    bMovingPoint_;
    bool                    bMovingLine_;
    bool                    bCtrl_;
    bool                    bShift_;
    bool                    bDrawAll_;

    QListViewItem*          pSelectedTrace_;
    MT_Vector2D             vPosMovingLine_;

    T_TracePtrVector         TraceVector_;
    QPopupMenu*             pMenu_;
    QPopupMenu*             pMenuTrace_;
    QPopupMenu*             pMenuPoint_;

    E_ListViewType          nType_;
    bool                    bActive_;

    MT_Vector2D             vOldPos_;
    MOS_Agent*              pAgent_;

    std::string             sFileName_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_TraceListView.inl"
#endif


#endif // __MOS_TraceListView_h_