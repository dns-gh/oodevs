//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Lima.h $
// $Author: Nld $
// $Modtime: 19/01/05 16:56 $
// $Revision: 11 $
// $Workfile: MOS_Lima.h $
//
//*****************************************************************************

#ifndef __MOS_Lima_h_
#define __MOS_Lima_h_

#include "MOS_Order_Def.h"

#include "MOS_Types.h"
#include "MOS_IDManager.h"
#include "MOS_ASN_Types.h"

enum E_StateLima
{   
    eStateLimaOk            = 0x00,
    eStateLimaCreated       = 0x01,
    eStateLimaCoordModified = 0x02,
    eStateLimaNameModified  = 0x04,
    eStateLimaDeleted       = 0x08
};

enum E_StateNetworkLima
{   
    eStateLimaNotRegistered     = 0,
    eStateLimaRegistering       = 1,
    eStateLimaRegistered        = 2
};

class MOS_LimaListViewItem;
class MOS_LimaListView;
class MOS_PointListViewItem;
class QListViewItem;

//=============================================================================
// Created: NLD 2002-08-08
//=============================================================================
class MOS_Lima
{
    MT_COPYNOTALLOWED( MOS_Lima );

public:
    static MOS_IDManager idManager_;

public:
     MOS_Lima( bool bEditable_ = true );
     MOS_Lima( const ASN1T_MsgLimaCreation& asnMsg );
    ~MOS_Lima();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MOS_LimaListView*, QListViewItem* >        T_LimaItemMap;
    typedef const T_LimaItemMap                                  CT_LimaItemMap;
    typedef T_LimaItemMap::iterator                              IT_LimaItemMap;
    typedef T_LimaItemMap::const_iterator                        CIT_LimaItemMap;
    //@}
    

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MIL_LimaID            GetID() const;
    QListViewItem*        GetItem( MOS_LimaListView& listView );
    CT_PointPtrVector&    GetListPoints() const;
    T_PointPtrVector&     GetListPoints();
    const std::string     GetName() const;
    void                  SetName( const std::string sText );
    void                  SetDefaultName();
    E_StateLima           GetState() const;
    void                  SetState( E_StateLima nStateLima );
    E_StateNetworkLima    GetNetworkState() const;
    void                  SetNetworkState( E_StateNetworkLima nStateNetworkLima );
    void                  RegisterListView( MOS_LimaListView& listview );
    void                  UnregisterListView( MOS_LimaListView& listview );
    void                  RegisterListViewItem( MOS_LimaListView* pListView, QListViewItem* pItem );
    void                  UnregisterListViewItem( MOS_LimaListView* pListView, QListViewItem* pItem );
    bool                  UpdateLimaToSim();
    std::string&          ConvertPositionValue( std::string& strValue );
    void                  DeleteListView();
    void                  DeleteListView( MOS_LimaListView& listView );
    void                  DeleteAllPoints();
    //@}

    //-------------------------------------------------------------------------
    /** @name Points*/
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& CreatePoint( const MT_Vector2D& vPos, MOS_PointListViewItem* pPointAfter = 0 );
    void               DeletePoint( MT_Vector2D& vPoint );
    void               RegisterPoint( MT_Vector2D& vPos );
    void               UnregisterPoint( const MT_Vector2D& vPoint );
    const MT_Vector2D& InsertPoint( const MT_Vector2D& vPosInsert, const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT*/
    //-------------------------------------------------------------------------
    //@{
    int Write( MT_OutputArchive_ABC& archive );
    int Read( MT_InputArchive_ABC& archive );
    //@}

    //-------------------------------------------------------------------------
    /** @name Func*/
    //-------------------------------------------------------------------------
    //@{
    E_FuncLimaType  GetTypeFunc() const;
    void            SetTypeFunc( E_FuncLimaType nTypeFunc );
    //@}

private:
    MIL_LimaID            nID_;

    T_LimaItemMap         limaItemMap_;
    
    T_PointPtrVector      pointVector_;
    std::string           sName_;

    bool                  bEditable_;
    E_StateLima           nStateLima_;
    E_StateNetworkLima    nStateNetworkLima_;
    E_FuncLimaType        nFuncType_;

    bool                  bCreatedByMOS_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_Lima.inl"
#endif

#endif // __MOS_Lima_h_
