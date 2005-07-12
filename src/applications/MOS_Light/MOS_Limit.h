//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Limit.h $
// $Author: Nld $
// $Modtime: 29/04/03 16:21 $
// $Revision: 9 $
// $Workfile: MOS_Limit.h $
//
//*****************************************************************************

#ifndef __MOS_Limit_h_
#define __MOS_Limit_h_

#include "MOS_Types.h"

#include "MOS_IDManager.h"
#include "MOS_ASN_Types.h"

class MOS_LimitListViewItem;
class MOS_LimitListView;
class MOS_PointListViewItem;
class QListViewItem;

enum E_StateLimit
{   
    eStateLimitOk            = 0x00,
    eStateLimitCreated       = 0x01,
    eStateLimitCoordModified = 0x02,
    eStateLimitNameModified  = 0x04,
    eStateLimitDeleted       = 0x08
};

enum E_StateNetworkLimit
{   
    eStateLimitNotRegistered     = 0,
    eStateLimitRegistering       = 1,
    eStateLimitRegistered        = 2
};


//=============================================================================
// Created: NLD 2002-08-08
//=============================================================================
class MOS_Limit
{
    MT_COPYNOTALLOWED( MOS_Limit );

public:
    static MOS_IDManager idManager_;

public:
     MOS_Limit( bool bEditable_ = true );
     MOS_Limit( const ASN1T_MsgLimitCreation& asnMsg );
    ~MOS_Limit();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MOS_LimitListView*, QListViewItem* >        T_LimitItemMap;
    typedef const T_LimitItemMap                                  CT_LimitItemMap;
    typedef T_LimitItemMap::iterator                              IT_LimitItemMap;
    typedef T_LimitItemMap::const_iterator                        CIT_LimitItemMap;
    //@}
    

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MIL_LimitID           GetID() const;
    QListViewItem*        GetItem( MOS_LimitListView& listView );
    CT_PointPtrVector&    GetListPoints() const;
    T_PointPtrVector&     GetListPoints();
    const std::string     GetName() const;
    void                  SetName( const std::string sText );
    void                  SetDefaultName();
    E_StateLimit          GetState() const;
    void                  SetState( E_StateLimit nStateLimit );
    E_StateNetworkLimit   GetNetworkState() const;
    void                  SetNetworkState( E_StateNetworkLimit nStateNetworkLimit );
    void                  RegisterListView( MOS_LimitListView& listview );
    void                  UnregisterListView( MOS_LimitListView& listview );
    void                  RegisterListViewItem( MOS_LimitListView* pListView, QListViewItem* pItem );
    void                  UnregisterListViewItem( MOS_LimitListView* pListView, QListViewItem* pItem );
    bool                  UpdateLimitToSim();
    std::string&          ConvertPositionValue( std::string& strValue );
    void                  DeleteListView();
    void                  DeleteListView( MOS_LimitListView& listView );
    void                  DeleteAllPoints();
    bool                  IsLocal() const;
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
    
private:
    bool                    bLocal_;
    MIL_LimitID             nID_;

    T_LimitItemMap          limitItemMap_;
    
    T_PointPtrVector        pointVector_;
    std::string             sName_;

    bool                    bEditable_;
    E_StateLimit            nStateLimit_;
    E_StateNetworkLimit     nStateNetworkLimit_;

    bool                    bCreatedByMOS_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_Limit.inl"
#endif

#endif // __MOS_Limit_h_
