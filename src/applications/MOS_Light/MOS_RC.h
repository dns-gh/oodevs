//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RC.h $
// $Author: Nld $
// $Modtime: 15/09/03 15:11 $
// $Revision: 17 $
// $Workfile: MOS_RC.h $
//
//*****************************************************************************

#ifndef __MOS_RC_h_
#define __MOS_RC_h_

#include <QListView.h>

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;
class MOS_RCListViewItem;
enum E_OrderConduiteID;

//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_RC
{
    MT_COPYNOTALLOWED( MOS_RC );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< E_OrderConduiteID >            T_OrderConduiteVector;
    typedef const T_OrderConduiteVector                  CT_OrderConduiteVector;
    typedef T_OrderConduiteVector::iterator              IT_OrderConduiteVector;
    typedef T_OrderConduiteVector::const_iterator        CIT_OrderConduiteVector;
    //@}

public:
     MOS_RC( MOS_Agent& agent );
    ~MOS_RC();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void                Initialize( const ASN1T_MsgCR& asnMsg );
    const std::string   BuildName( const ASN1T_MsgCR& asnMsg ) const;

    void Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D&    GetPos()  const;
    int                   GetTime() const;
    MOS_Agent&            GetAgent();

    const std::string&           GetTitle         () const;
    const T_StringVector&        GetTextFields    () const; 
    const T_OrderConduiteVector& GetOrderConduites()const;
    //@}

    void SetRCItem( MOS_RCListViewItem* pItem );
    
private:
    std::string             strTitle_;
    T_StringVector          textVector_;
    MOS_Agent&              agent_;
    MT_Vector2D             vPos_;
    int                     nTime_;
    
    MIL_LimaID              nLimaID_;
    MIL_AgentID             nTargetAgentID_;


    T_OrderConduiteVector   conduiteOrderVector_;

    MOS_RCListViewItem*     pItem_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_RC.inl"
#endif

#endif // __MOS_RC_h_
