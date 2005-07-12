//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCEditor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_RCEditor.h $
//
//*****************************************************************************

#ifndef __MOS_RCEditor_h_
#define __MOS_RCEditor_h_

#include "MOS_Types.h"

#include <QFrame.h>
#include <QWidget.h>

class MOS_RCListView;
class QWidget;
class MOS_Agent;
class MOS_AttrEditor;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_RCEditor : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_RCEditor );


public:
    explicit MOS_RCEditor( MOS_AttrEditor& attrEditor, QWidget* pParent = 0 );
    ~MOS_RCEditor();

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
    MOS_RCListView&         GetRCListView () const;
    bool                    IsActive        ( bool bMove );
    void                    SetAgent        ( MOS_Agent* pAgent );
    //@}


private:
    MOS_RCListView*         pRCListView_;
    MOS_AttrEditor&         attrEditor_;
    QWidget*                pWidgetTab_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_RCEditor.inl"
#endif


#endif // __MOS_RCEditor_h_