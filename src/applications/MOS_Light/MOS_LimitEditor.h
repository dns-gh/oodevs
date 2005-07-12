//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitEditor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_LimitEditor.h $
//
//*****************************************************************************

#ifndef __MOS_LimitEditor_h_
#define __MOS_LimitEditor_h_

#include "MOS_Types.h"

#include <QFrame.h>

class MOS_LimitListView;
class QWidget;
class MOS_Agent;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimitEditor : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimitEditor );


public:
    explicit MOS_LimitEditor( QWidget* pParent = 0 );
    virtual ~MOS_LimitEditor();

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_LimitListView&  GetLimitListView() const;
    bool                IsActive        ( bool bMove );
    void                SetAgent        ( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name Exit*/
    //-------------------------------------------------------------------------
    //@{
    void Accept();
    void Cancel();
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT*/
    //-------------------------------------------------------------------------
    //@{
    void Read();
    void Write();
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
};

#ifdef MOS_USE_INLINE
#	include "MOS_LimitEditor.inl"
#endif


#endif // __MOS_LimitEditor_h_