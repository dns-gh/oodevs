//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBPionListViewItem.h $
// $Author: Nld $
// $Modtime: 12/11/03 10:00 $
// $Revision: 3 $
// $Workfile: MOS_ODBPionListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_ODBPionListViewItem_h_
#define __MOS_ODBPionListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "GFX/GFX_Color.h"

class MOS_ODBTypePion;
class MOS_ODBTypeAutomate;
class QListViewItem;
class MOS_Agent;


class MOS_ODBPionListViewItem;

typedef std::vector< MOS_ODBPionListViewItem* >  T_ODBPionPtrVector;
typedef const T_ODBPionPtrVector              CT_ODBPionPtrVector;
typedef T_ODBPionPtrVector::iterator          IT_ODBPionPtrVector;
typedef T_ODBPionPtrVector::const_iterator    CIT_ODBPionPtrVector;
typedef T_ODBPionPtrVector::reverse_iterator  RIT_ODBPionPtrVector;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ODBPionListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_ODBPionListViewItem );

public:
    MOS_ODBPionListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, E_TypeArme nTypeArme, MOS_ODBTypeAutomate* pTypeAutomate, MOS_ODBTypePion* pTypePion, const MT_Rect& rect, uint nIdAutomate );
    virtual ~MOS_ODBPionListViewItem();

    void                        CreateMenu      ( QPopupMenu* pMenu );
    void                        ExecuteMenu     ( int nClicked );
    void                        DeleteAllPions  ();
    void                        DeletePion      ( MOS_ODBPionListViewItem* pPion );

    void                        CreatePions     ( uint nValue );
    uint                        GetNbSons       () const;
    void                        SetName         ();
    void                        SetRandomPos    ( const MT_Rect& rect );
    void                        Draw            ( bool bSelected );
    void                        DrawRectLimit   ( bool bSelected );

    bool                        StartMoveExtent ( const MT_Vector2D& vPosGL );
    bool                        MoveExtent      ( const MT_Vector2D& vPosGL );
    bool                        EndMoveExtent   ();
    void                        SetInside       ( const MT_Rect& rectLimit, bool bForce = false );
    void                        UpdateAll       ();
    void                        SetCenter       ( const MT_Vector2D& vDeltaPos );
    void                        FitParent       ( const MT_Rect& rectLimit );
    void                        ClipLimit       ();
    void                        WritePions      ( MT_OutputArchive_ABC& archive );
    uint                        GetNbPions      ();


private:

    E_TypeCamp                      nTypeCamp_;
    E_TypeArme                      nTypeArme_;
    MOS_ODBTypeAutomate*            pTypeAutomate_;
    MOS_ODBTypePion*                pTypePion_;
    T_ODBPionPtrVector              ODBPionPtrVector_;
    MT_Vector2D                     vPos_;
    MT_Rect                         rectLimit_;
    bool                            bEditLeft_;
    bool                            bEditRight_;
    bool                            bEditCenter_;
    uint                            nWantedPion_;
    GFX_Color                       colorRect_;
    uint                            nId_;
    uint                            nIdAutomate_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ODBPionListViewItem.inl"
#endif

#endif // __MOS_ODBPionListViewItem_h_
