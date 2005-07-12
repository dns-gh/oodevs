//*****************************************************************************
//
// $Created: FBD 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_ODBListView.h $
//
//*****************************************************************************

#ifndef __MOS_ODBListView_h_
#define __MOS_ODBListView_h_

#include "MOS_Types.h"

#include <QListView.h>
#include "MOS_GLMouse_ABC.h"
#include "GFX/GFX_Color.h"

class MOS_ODBCampListViewItem;
class QWidget;
class MOS_ListViewItem_ABC;

//*****************************************************************************
// Created: FBD 03-07-24
//*****************************************************************************
class MOS_ODBListView   : public QListView
                        , public MOS_GLMouse_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ODBListView );


private:
    typedef std::map< E_TypeCamp, MOS_ODBCampListViewItem* >        T_ODBCampPtrMap;
    typedef const T_ODBCampPtrMap                                  CT_ODBCampPtrMap;
    typedef T_ODBCampPtrMap::iterator                              IT_ODBCampPtrMap;
    typedef T_ODBCampPtrMap::const_iterator                        CIT_ODBCampPtrMap;
    typedef T_ODBCampPtrMap::reverse_iterator                      RIT_ODBCampPtrMap;

public:
    //-------------------------------------------------------------------------
    /** @name Constructors */
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_ODBListView( QWidget* pParent );
    virtual ~MOS_ODBListView();
    //@}

    //-------------------------------------------------------------------------
    /** @name Init/Reset */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Arme */
    //-------------------------------------------------------------------------
    //@{
    MOS_ODBCampListViewItem*    CreateCamp( E_TypeCamp nTypeCamp );
    void                        DeleteCamp( E_TypeCamp nTypeCamp );
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw();
    void DrawRectLimit( bool bSelected );
    void FitParent( const MT_Rect& rectLimit );

    bool IsActive( bool bMove );
    bool RMBPressEvent( const MT_Vector2D& vPos );
    bool LMBPressEvent( const MT_Vector2D& vPos );
    void MouseMoveEvent( const MT_Vector2D& vPos );
    void RMBReleaseEvent( const MT_Vector2D& vPos );
    void LMBReleaseEvent( const MT_Vector2D& vPos );
    void SetName();
    uint GetNbSons();
    void SetRandomPos( const MT_Rect& rect );
    bool StartMoveExtent( const MT_Vector2D& vPosGL );
    bool MoveExtent( const MT_Vector2D& vPosGL );
    bool EndMoveExtent();
    void SetInside( const MT_Rect& rectLimit );
    void SetCenter( const MT_Vector2D& vDeltaPos );
    void UpdateAll();
    void ClipLimit();
    void keyPressEvent( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
    uint GetNbAutomates();
    uint GetNbPions();
    uint GetIdAutomate();
    uint GetIdPion();

    //@}


protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotContextMenuRequested   ( QListViewItem* pItem, const QPoint& point );
    void SlotItemPressed            ( QListViewItem* pItem );
    void SlotWriteODB               ();
    //@}

private:
    MOS_ListViewItem_ABC*   pTotalPion_;
    T_ODBCampPtrMap         campPtrMap_;
    MT_Rect                 rectLimit_;
    MT_Vector2D             vPos_;
    bool                    bEditLeft_;
    bool                    bEditRight_;
    bool                    bEditCenter_;
    GFX_Color               colorRect_;
    bool                    bShift_;
    bool                    bCtrl_;
    uint                    nLastIdAutomate_;
    uint                    nLastIdPion_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_ODBListView.inl"
#endif

                        
#endif // __MOS_ODBListView_h_