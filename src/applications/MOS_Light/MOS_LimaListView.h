//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 20 $
// $Workfile: MOS_LimaListView.h $
//
//*****************************************************************************

#ifndef __MOS_LimaListView_h_
#define __MOS_LimaListView_h_

#include "MOS_Types.h"

#include <QListView.h>
#include "MOS_ListViewItem_ABC.h"
#include "MOS_GLMouse_ABC.h"

#include "MOS_LineManager.h"

class QWidget;
class QPopupMenu;
class QListViewItem;

class MOS_LimaListViewItem;
class MOS_LimaCheckListItem;
class MOS_PointListViewItem;
class MOS_Lima;
class MOS_Agent;


//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimaListView : public QListView
                       , public MOS_GLMouse_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimaListView );

private: 
    typedef std::vector< QListViewItem* >           T_LimaPtrVector;
    typedef const T_LimaPtrVector                   CT_LimaPtrVector;
    typedef T_LimaPtrVector::iterator               IT_LimaPtrVector;
    typedef T_LimaPtrVector::const_iterator         CIT_LimaPtrVector;
    typedef T_LimaPtrVector::reverse_iterator       RIT_LimaPtrVector;

public:


    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_LimaListView( QWidget* pParent, E_ListViewType nType );
    virtual ~MOS_LimaListView();
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
    QListViewItem*         CreateLima              ( MOS_Lima& lima );
    void                   DeleteLima              ( QListViewItem& lima );
    void                   RegisterLima            ( QListViewItem& lima );
    void                   UnregisterLima          ( QListViewItem& lima );
    QListViewItem*         CreateLimaFromInterface ();
    void                   DeleteLimaFromInterface ( QListViewItem& lima );
    MOS_PointListViewItem* CreatePointFromInterface( QListViewItem& lima, MOS_PointListViewItem* pPoint = 0, MT_Vector2D* pPos = 0 );
    void                   DeletePointFromInterface( QListViewItem& lima, MOS_PointListViewItem& point );
    void                   SetPosPoint             ( QListViewItem& lima, MOS_PointListViewItem& point, const MT_Vector2D& vPos );
    void                   SetPosLima              ( QListViewItem& lima, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine );
    void                   LocOnPoint              ( MOS_PointListViewItem* pPoint, bool bSelectList = true );
    E_ListViewType         GetType                 () const;
    void                   CreateLimaFromSelector  ( MOS_LimaListView* pSelector );
    void                   GetTaggedLimas          ( MOS_LineManager::T_LimaSet& vectorLima );
    void                   SetAgent                ( MOS_Agent* pAgent );
    MOS_Agent*             GetAgent                ();
    void                   SetOldPos               ( const MT_Vector2D& vPos );
    MT_Vector2D&           GetOldPos               ();
    T_LimaPtrVector&       GetListLima            ();

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
    void WriteMsg( ASN1T_ListOID& asnLimas, std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}
    
    //-------------------------------------------------------------------------
    /** @name */
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
    void EnterLimaEditor         ();
    void EnterLimaSelector       ();
    //@}

private:
    bool                    bMovingPoint_;
    bool                    bMovingLine_;
    bool                    bCtrl_;
    bool                    bShift_;
    bool                    bDrawAll_;

    MOS_PointListViewItem*  pSelectedPoint_;
    QListViewItem*          pSelectedLima_;
    MT_Vector2D             vPosMovingLine_;

    T_LimaPtrVector         limaVector_;
    QPopupMenu*             pMenu_;
    QPopupMenu*             pMenuLima_;
    QPopupMenu*             pMenuPoint_;

    E_ListViewType          nType_;
    bool                    bActive_;

    MT_Vector2D             vOldPos_;
    MOS_Agent*              pAgent_;

    std::string             sFileName_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_LimaListView.inl"
#endif


#endif // __MOS_LimaListView_h_