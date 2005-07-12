//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaEditor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_LimaEditor.h $
//
//*****************************************************************************

#ifndef __MOS_LimaEditor_h_
#define __MOS_LimaEditor_h_

#include "MOS_Types.h"

#include <QFrame.h>

class MOS_LimaListView;
class QWidget;
class MOS_Agent;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_LimaEditor : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LimaEditor );


public:
    explicit MOS_LimaEditor( QWidget* pParent = 0 );
    virtual ~MOS_LimaEditor();

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
    MOS_LimaListView&       GetLimaListView () const;
    bool                    IsActive        ( bool bMove );
    void                    SetAgent        ( MOS_Agent* pAgent );
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
    MOS_LimaListView*   pLimaListView_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_LimaEditor.inl"
#endif


#endif // __MOS_LimaEditor_h_