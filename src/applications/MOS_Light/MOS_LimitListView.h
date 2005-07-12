//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 20 $
// $Workfile: MOS_LimitListView.h $
//
//*****************************************************************************

#ifndef __MOS_LimitListView_h_
#define __MOS_LimitListView_h_

#include "MOS_Types.h"

#include <QListView.h>
#include "MOS_ListViewItem_ABC.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_LineManager.h"

class QWidget;
class QPopupMenu;
class QListViewItem;

class MOS_LimitListViewItem;
class MOS_LimitCheckListItem;
class MOS_PointListViewItem;
class MOS_Limit;
class MOS_Agent;

   
//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimitListView : public QListView
                       , public MOS_GLMouse_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimitListView );

private: 
    typedef std::vector< QListViewItem* >           T_LimitPtrVector;
    typedef const T_LimitPtrVector                   CT_LimitPtrVector;
    typedef T_LimitPtrVector::iterator               IT_LimitPtrVector;
    typedef T_LimitPtrVector::const_iterator         CIT_LimitPtrVector;
    typedef T_LimitPtrVector::reverse_iterator       RIT_LimitPtrVector;

public:


    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_LimitListView( QWidget* pParent, E_ListViewType nType );
    virtual ~MOS_LimitListView();
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
    QListViewItem*         CreateLimit              ( MOS_Limit& Limit );
    void                   DeleteLimit              ( QListViewItem& Limit );
    void                   RegisterLimit            ( QListViewItem& Limit );
    void                   UnregisterLimit          ( QListViewItem& Limit );
    QListViewItem*         CreateLimitFromInterface ();
    void                   DeleteLimitFromInterface ( QListViewItem& Limit );
    MOS_PointListViewItem* CreatePointFromInterface ( QListViewItem& Limit, MOS_PointListViewItem* pPoint = 0, MT_Vector2D* pPos = 0 );
    void                   DeletePointFromInterface ( QListViewItem& Limit, MOS_PointListViewItem& point );
    void                   SetPosPoint              ( QListViewItem& Limit, MOS_PointListViewItem& point, const MT_Vector2D& vPos );
    void                   SetPosLimit              ( QListViewItem& Limit, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    void                   LocOnPoint               ( MOS_PointListViewItem* pPoint, bool bSelectList = true );
    E_ListViewType         GetType                  () const;
    void                   CreateLimitFromSelector  ( MOS_LimitListView* pSelector );
    void                   GetTaggedLimits          ( MOS_LineManager::T_LimitSet& vectorLimit );
    void                   SetOldPos                ( const MT_Vector2D& vPos );
    MT_Vector2D&           GetOldPos                ();
    void                   SetAgent                 ( MOS_Agent* pAgent );
    MOS_Agent*             GetAgent                 ();
    T_LimitPtrVector&      GetListLimit             ();

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
    void keyPressEvent  ( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( ASN1T_OID& asnLeftLimitID, ASN1T_OID& asnRightLimitID, std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive  );

    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT*/
    //-------------------------------------------------------------------------
    //@{
    int Read    ();
    int Write   () const;
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point );
    void SlotItemPressed         ( QListViewItem* pItem );
    void SlotItemDoubleClicked   ( QListViewItem* pItem );
    void SlotSetShowAll          ( bool );
    void SlotItemRenamed         ( QListViewItem* pItem, int nCol, const QString& sText );
    void EnterLimitEditor        ();
    void EnterLimitSelector      ();
    //@}

private:
    bool                    bMovingPoint_;
    bool                    bMovingLine_;
    bool                    bCtrl_;
    bool                    bShift_;
    bool                    bDrawAll_;
    bool                    bCreatedPoint_;

    MOS_PointListViewItem*  pSelectedPoint_;
    QListViewItem*          pSelectedLimit_;
    MT_Vector2D             vPosMovingLine_;
    MT_Vector2D             vOldPos_;
    T_LimitPtrVector        limitVector_;
    QPopupMenu*             pMenu_;
    QPopupMenu*             pMenuLimit_;
    QPopupMenu*             pMenuPoint_;

    E_ListViewType          nType_;
    bool                    bActive_;

    MOS_Agent*              pAgent_;
    std::string             sFileName_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_LimitListView.inl"
#endif


#endif // __MOS_LimitListView_h_