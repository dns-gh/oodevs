//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBCampListViewItem.h $
// $Author: Nld $
// $Modtime: 12/11/03 10:00 $
// $Revision: 3 $
// $Workfile: MOS_ODBCampListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_ODBCampListViewItem_h_
#define __MOS_ODBCampListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_ODBArmeListViewItem.h"
#include "GFX/GFX_Color.h"

class QListView;
class QListViewItem;
class MOS_ODBListView;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ODBCampListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_ODBCampListViewItem );
private:
    typedef std::vector< QListViewItem* >          T_ArmePtrVector;
    typedef const T_ArmePtrVector              CT_ArmePtrVector;
    typedef T_ArmePtrVector::iterator          IT_ArmePtrVector;
    typedef T_ArmePtrVector::const_iterator    CIT_ArmePtrVector;
    typedef T_ArmePtrVector::reverse_iterator  RIT_ArmePtrVector;

    typedef std::map< E_TypeArme, MOS_ODBArmeListViewItem* >        T_ODBArmePtrMap;
    typedef const T_ODBArmePtrMap                                  CT_ODBArmePtrMap;
    typedef T_ODBArmePtrMap::iterator                              IT_ODBArmePtrMap;
    typedef T_ODBArmePtrMap::const_iterator                        CIT_ODBArmePtrMap;
    typedef T_ODBArmePtrMap::reverse_iterator                      RIT_ODBArmePtrMap;


public:
    MOS_ODBCampListViewItem( QListViewItem& item, E_TypeCamp nType, const MT_Rect& rectLimit );
   
    
    virtual ~MOS_ODBCampListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    QListViewItem*      GetFirstArme() const;
    const std::string   GetName() const;
    void                SetName();
    uint                GetNbSons();
    E_TypeCamp          GetTypeCamp() const;
    void                Initialize();
    void                Terminate();
    void                Draw( bool bSelected );
    void                DrawRectLimit( bool bSelected );
    //@}

    //-------------------------------------------------------------------------
    /** @name Armes*/
    //-------------------------------------------------------------------------
    //@{
    void            RegisterArme    ( QListViewItem& Arme, MOS_ODBListView* pListview, QListViewItem* pArmeAfter = 0 );
    void            UnregisterArme  ( const QListViewItem& Arme );
    QListViewItem&  CreateArme      ( E_TypeArme nType );
    void            DeleteArme      ( E_TypeArme nType );
    std::string     BuildNameCamp   ( E_TypeCamp nTypeCamp );
    void            SetRandomPos    ( const MT_Rect& rect );
    bool            StartMoveExtent ( const MT_Vector2D& vPosGL );
    bool            MoveExtent      ( const MT_Vector2D& vPosGL );
    bool            EndMoveExtent   ();
    void            SetInside       ( const MT_Rect& rectLimit );
    void            UpdateAll       ();
    void            SetCenter       ( const MT_Vector2D& vDeltaPos );
    void            FitParent       ( const MT_Rect& rectLimit );
    void            ClipLimit       ();
    void            WriteAutomates  ( MT_OutputArchive_ABC& archive );
    void            WritePions      ( MT_OutputArchive_ABC& archive );
    uint            GetNbAutomates  ();
    uint            GetNbPions      ();
   //@}




private:
    E_TypeCamp                      nTypeCamp_;
    T_ODBArmePtrMap                 armePtrMap_;
    MT_Vector2D                     vPos_;
    MT_Rect                         rectLimit_;
    bool                            bEditLeft_;
    bool                            bEditRight_;
    bool                            bEditCenter_;
    GFX_Color                       colorRect_;

};


#ifdef MOS_USE_INLINE
#   include "MOS_ODBCampListViewItem.inl"
#endif

#endif // __MOS_ODBCampListViewItem_h_
