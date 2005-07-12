//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitSelector.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_LimitSelector.h $
//
//*****************************************************************************

#ifndef __MOS_LimitSelector_h_
#define __MOS_LimitSelector_h_

#include "MOS_Types.h"

#include <QFrame.h>

class MOS_LimitListView;
class MOS_LimitMission;
class QWidget;
class MOS_Agent;


//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimitSelector : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimitSelector );

public:
    explicit MOS_LimitSelector( QWidget* pParent = 0 );
    virtual ~MOS_LimitSelector();

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
    MOS_LimitListView&  GetLimitListView() const;
    void                SetLimitMission ( MOS_LimitListView& LimitMission );
    void                SetAgent        ( MOS_Agent* pAgent );
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
    MOS_LimitListView*   pLimitListView_;
    MOS_LimitListView*   pMissionListView_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_LimitSelector.inl"
#endif


#endif // __MOS_LimitSelector_h_