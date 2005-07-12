//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaSelector.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_LimaSelector.h $
//
//*****************************************************************************

#ifndef __MOS_LimaSelector_h_
#define __MOS_LimaSelector_h_

#include "MOS_Types.h"

#include <QFrame.h>

class MOS_LimaListView;
class MOS_LimaMission;
class QWidget;
class MOS_Agent;


//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimaSelector : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimaSelector );

public:
    explicit MOS_LimaSelector( QWidget* pParent = 0 );
    virtual ~MOS_LimaSelector();

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool IsActive( bool bMove );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_LimaListView&       GetLimaListView () const;
    void                    SetLimaMission  ( MOS_LimaListView& limaMission );
    void                    SetAgent        ( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name Exit*/
    //-------------------------------------------------------------------------
    //@{
    void Accept();
    void Cancel();
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotAccept();
    void SlotReject();
    //@}


private:
    MOS_LimaListView*   pLimaListView_;
    MOS_LimaListView*   pMissionListView_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_LimaSelector.inl"
#endif


#endif // __MOS_LimaSelector_h_