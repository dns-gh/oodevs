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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBArmeListViewItem.h $
// $Author: Nld $
// $Modtime: 12/11/03 10:00 $
// $Revision: 3 $
// $Workfile: MOS_ODBArmeListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_ODBArmeListViewItem_h_
#define __MOS_ODBArmeListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_ODBAutomateListViewItem.h"
#include "MOS_ODBTypeAutomate.h"
#include "GFX/GFX_Color.h"

class QListView;
class QListViewItem;
class MOS_ODBListView;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ODBArmeListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_ODBArmeListViewItem );
private:
    typedef std::vector< MOS_ODBAutomateListViewItem* >             T_ODBAutomatePtrVector;
    typedef const T_ODBAutomatePtrVector                           CT_ODBAutomatePtrVector;
    typedef T_ODBAutomatePtrVector::iterator                       IT_ODBAutomatePtrVector;
    typedef T_ODBAutomatePtrVector::const_iterator                 CIT_ODBAutomatePtrVector;
    typedef T_ODBAutomatePtrVector::reverse_iterator               RIT_ODBAutomatePtrVector;

    typedef std::vector< MOS_ODBTypeAutomate* >                     T_ODBTypeAutomatePtrVector;
    typedef const T_ODBTypeAutomatePtrVector                           CT_ODBTypeAutomatePtrVector;
    typedef T_ODBTypeAutomatePtrVector::iterator                       IT_ODBTypeAutomatePtrVector;
    typedef T_ODBTypeAutomatePtrVector::const_iterator                 CIT_ODBTypeAutomatePtrVector;
    typedef T_ODBTypeAutomatePtrVector::reverse_iterator               RIT_ODBTypeAutomatePtrVector;

public:
    MOS_ODBArmeListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, E_TypeArme nTypeArme, const MT_Rect& rectLimit );
    virtual ~MOS_ODBArmeListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    QListViewItem*    GetFirstAutomate() const;
    const std::string GetName() const;
    E_TypeArme        GetTypeArme() const;
    void              Initialize();
    void              Terminate();
    void              SetRandomPos( const MT_Rect& rect );
    void              UpdateAll();

    //@}

    //-------------------------------------------------------------------------
    /** @name Automates*/
    //-------------------------------------------------------------------------
    //@{
    QListViewItem&  CreateAutomate    ( MOS_ODBTypeAutomate* pTypeAutomate );
    void            DeleteAutomate    ( QListViewItem* pAutomate );
    void            CreateTypeAutomate( const std::string& sName, const std::string& sSymbol );
    void            DeleteAllTypeAutomate();
    std::string     BuildNameArme     ( E_TypeArme nTypeArme );
    void            SetName();
    uint            GetNbSons();
    void            Draw( bool bSelected );
    void            DrawRectLimit( bool bSelected );

    bool            StartMoveExtent ( const MT_Vector2D& vPosGL );
    bool            MoveExtent      ( const MT_Vector2D& vPosGL );
    bool            EndMoveExtent   ();
    void            SetInside       ( const MT_Rect& rectLimit );
    void            SetCenter       ( const MT_Vector2D& vDeltaPos );
    void            FitParent       ( const MT_Rect& rect );
    void            ClipLimit       ();
    void            WriteAutomates  ( MT_OutputArchive_ABC& archive );
    void            WritePions      ( MT_OutputArchive_ABC& archive );
    uint            GetNbAutomates  ();
    uint            GetNbPions      ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Menus*/
    //-------------------------------------------------------------------------
    //@{
    void CreateMenu( QPopupMenu* pMenu );
    void ExecuteMenu( int nClicked );
    //@}


private:
    E_TypeCamp                      nTypeCamp_;
    E_TypeArme                      nTypeArme_;
    T_ODBAutomatePtrVector          ODBAutomatePtrVector_;
    T_ODBTypeAutomatePtrVector      ODBTypeAutomatePtrVector_;
    MT_Vector2D                     vPos_;
    MT_Rect                         rectLimit_;
    bool                            bEditLeft_;
    bool                            bEditRight_;
    bool                            bEditCenter_;
    GFX_Color                       colorRect_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ODBArmeListViewItem.inl"
#endif

#endif // __MOS_ODBArmeListViewItem_h_
