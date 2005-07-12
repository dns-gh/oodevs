//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentListViewItem_ABC.h $
// $Author: Fbd $
// $Modtime: 27/08/03 14:55 $
// $Revision: 6 $
// $Workfile: MOS_AgentListViewItem_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_AgentListViewItem_ABC_h_
#define __MOS_AgentListViewItem_ABC_h_

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_AgentListViewItem_ABC : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentListViewItem_ABC );

public:
    MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView );
    MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView, QListViewItem& itemParent );
    MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent, QListViewItem& itemAfter );
    MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent );
    
    virtual ~MOS_AgentListViewItem_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Agent&                      GetAgent() const;
    const std::string               GetName () const;
    virtual void                    SetName ( const std::string& sName );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( ASN1T_OID& asnMsg, std::string& sParam );
    //@}
    

private:
    MOS_Agent& agent_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AgentListViewItem_ABC.inl"
#endif

#endif // __MOS_AgentListViewItem_ABC_h_
