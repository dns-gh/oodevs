//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBAutomateListViewItem.h $
// $Author: Nld $
// $Modtime: 12/11/03 10:00 $
// $Revision: 3 $
// $Workfile: MOS_ODBAutomateListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_ODBAutomateListViewItem_h_
#define __MOS_ODBAutomateListViewItem_h_

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_ODBPionListViewItem.h"
#include "MOS_ODBTypePion.h"
#include "MOS_ODBTypeAutomate.h"
#include "GFX/GFX_Color.h"

class QListView;
class QListViewItem;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ODBAutomateListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_ODBAutomateListViewItem );

private:
    typedef std::vector< MOS_ODBTypePion* >  T_ODBTypePionPtrVector;
    typedef const T_ODBTypePionPtrVector              CT_ODBTypePionPtrVector;
    typedef T_ODBTypePionPtrVector::iterator          IT_ODBTypePionPtrVector;
    typedef T_ODBTypePionPtrVector::const_iterator    CIT_ODBTypePionPtrVector;
    typedef T_ODBTypePionPtrVector::reverse_iterator  RIT_ODBTypePionPtrVector;

public:
    MOS_ODBAutomateListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, E_TypeArme nTypeArme, MOS_ODBTypeAutomate* pTypeAutomate, const MT_Rect& rect );
    virtual ~MOS_ODBAutomateListViewItem();

    //-------------------------------------------------------------------------
    /** @name Pions*/
    //-------------------------------------------------------------------------
    //@{
    MOS_ODBPionListViewItem&    CreatePion    ( MOS_ODBTypePion* pTypePion );
    void                        DeletePion    ( MOS_ODBPionListViewItem* pPion );
    void                        CreateTypePion( const std::string& sName, const std::string& sSymbol );
    void                        DeleteAllTypePion();
    void                        SetName();
    uint                        GetNbSons();
    void                        SetRandomPos( const MT_Rect& rect );
    void                        Draw( bool bSelected );
    void                        DrawRectLimit( bool bSelected );
    bool                        StartMoveExtent ( const MT_Vector2D& vPosGL );
    bool                        MoveExtent      ( const MT_Vector2D& vPosGL );
    bool                        EndMoveExtent   ();
    void                        SetInside       ( const MT_Rect& rectLimit );
    void                        UpdateAll       ();
    void                        SetCenter       ( const MT_Vector2D& vDeltaPos );
    void                        FitParent       ( const MT_Rect& rect );
    void                        ClipLimit       ();
    void                        WriteAutomate   ( MT_OutputArchive_ABC& archive );
    void                        WritePions      ( MT_OutputArchive_ABC& archive );
    std::string                 GetNameTypeCamp ( E_TypeCamp nTypeCamp );
    uint                        GetNbAutomates  ();
    uint                        GetNbPions      ();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Menus*/
    //-------------------------------------------------------------------------
    //@{
    void CreateMenu( QPopupMenu* pMenu );
    void ExecuteMenu( int nClicked );
    //@}

private:
    E_TypeCamp                  nTypeCamp_;
    E_TypeArme                  nTypeArme_;
    MOS_ODBTypeAutomate*        pTypeAutomate_;
    
    T_ODBPionPtrVector          ODBPionPtrVector_;
    T_ODBTypePionPtrVector      ODBTypePionPtrVector_;
    MT_Vector2D                 vPos_;
    MT_Rect                     rectLimit_;
    bool                        bEditLeft_;
    bool                        bEditRight_;
    bool                        bEditCenter_;
    GFX_Color                   colorRect_;
    uint                        nId_;
   
};


#ifdef MOS_USE_INLINE
#   include "MOS_ODBAutomateListViewItem.inl"
#endif

#endif // __MOS_ODBAutomateListViewItem_h_
