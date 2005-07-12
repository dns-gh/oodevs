//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_Trace.h $
// $Author: Agn $
// $Modtime: 3/06/03 12:08 $
// $Revision: 4 $
// $Workfile: MOS_Trace.h $
//
//*****************************************************************************

#ifndef __MOS_Trace_h_
#define __MOS_Trace_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include <QListView.h>

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"

class MOS_Agent;


//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_Trace
{
    MT_COPYNOTALLOWED( MOS_Trace );

public:
     MOS_Trace( MOS_Agent& agent );
    ~MOS_Trace();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void                Initialize( DIN::DIN_Input& input );
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& GetPos() const;
    const std::string& GetName() const;
    int                GetTime() const;
    MOS_Agent&         GetAgent();
    //@}


    void SetTraceItem( QListViewItem* pItem );
    
private:
    std::string             sText_; 
    MOS_Agent&              agent_;
    MT_Vector2D             vPos_;
    int                     nTime_;
    
    QListViewItem*          pItem_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_Trace.inl"
#endif

#endif // __MOS_Trace_h_
