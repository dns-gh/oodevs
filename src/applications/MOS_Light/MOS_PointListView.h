//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PointListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 16 $
// $Workfile: MOS_PointListView.h $
//
//*****************************************************************************

#ifndef __MOS_PointListView_h_
#define __MOS_PointListView_h_

#include "MOS_Types.h"

#include <QListView.h>
#include "MOS_ListViewItem_ABC.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_MissionTool_ABC.h"

class QWidget;
class QPopupMenu;
class MOS_PointListViewItem;
class QListViewItem;
class MOS_Agent;
   
//*****************************************************************************
// Created: FBD 03-01-13
//*****************************************************************************
class MOS_PointListView : public QListView
                        , public MOS_GLMouse_ABC
                        , public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_PointListView );


public: 
    typedef std::vector< MOS_PointListViewItem* >    T_PointPtrVector;
    typedef const T_PointPtrVector                   CT_PointPtrVector;
    typedef T_PointPtrVector::iterator               IT_PointPtrVector;
    typedef T_PointPtrVector::const_iterator         CIT_PointPtrVector;
    typedef T_PointPtrVector::reverse_iterator       RIT_PointPtrVector;

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_PointListView( QWidget* pParent, const std::string& strName, E_ListViewType nType );
    virtual ~MOS_PointListView();
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{


    MOS_PointListViewItem& CreatePoint              ( const MT_Vector2D& vPoint, MOS_PointListViewItem* pPointAfter = 0 );
    void                   DeletePoint              ( MOS_PointListViewItem& point );
    void                   RegisterPoint            ( MOS_PointListViewItem& point, MOS_PointListViewItem* pPointAfter = 0 );
    void                   UnregisterPoint          ( const MOS_PointListViewItem& point );
    MOS_PointListViewItem* CreatePointFromInterface ( MOS_PointListViewItem& point );
    void                   DeletePointFromInterface ( MOS_PointListViewItem& point );
    void                   DeleteAllPoints          ();
    void                   SetPosPoint              ( MOS_PointListViewItem& point, const MT_Vector2D& vPos );

    void                   LocOnPoint               ( MOS_PointListViewItem* point, bool bSelectList = true );
    E_ListViewType         GetType                  () const;
    MOS_PointListViewItem* GetPointBefore           ( const MOS_PointListViewItem& point ) const;
    MOS_PointListViewItem* GetPointAfter            ( const MOS_PointListViewItem& point ) const;
    MOS_PointListViewItem* GetFirstPoint            () const;
    MOS_PointListViewItem* IsInsideLine             ( const MT_Vector2D& vPos ) const;

    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Draw                 () = 0;
    virtual bool IsActive             ( bool bMove ) = 0;
    virtual bool RMBPressEvent        ( const MT_Vector2D& vPos ) = 0;
    virtual bool LMBPressEvent        ( const MT_Vector2D& vPos ) = 0;
    virtual void MouseMoveEvent       ( const MT_Vector2D& vPos ) = 0;
    virtual void RMBReleaseEvent      ( const MT_Vector2D& vPos ) = 0;
    virtual void LMBReleaseEvent      ( const MT_Vector2D& vPos ) = 0;
    //@}

     //-------------------------------------------------------------------------
    /** @name Events*/
    //-------------------------------------------------------------------------
    //@{
    void keyPressEvent ( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
    //@}

    //-------------------------------------------------------------------------
    /** @name Points Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual MT_Vector2D            GetCenterPoint( MOS_PointListViewItem* pPoint ) = 0;
    virtual MOS_PointListViewItem* IsInsidePoint    ( const MT_Vector2D& vPos ) const = 0;
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void         SlotItemPressed         ( QListViewItem* pItem );
    virtual void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point ) = 0;
    //@}


protected:
    MT_Vector2D             vPosMovingLine_;
    T_PointPtrVector        pointVector_;
    MOS_PointListViewItem*  pSelectedPoint_;
    bool                    bMovingPoint_;
    bool                    bShift_;
    bool                    bCtrl_;


private:
    MOS_PointListViewItem*  pPostStart_;
    E_ListViewType          nType_;
private:
};

#ifdef MOS_USE_INLINE
#	include "MOS_PointListView.inl"
#endif


#endif // __MOS_PointListView_h_