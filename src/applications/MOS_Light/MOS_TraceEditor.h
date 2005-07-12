//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceEditor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_TraceEditor.h $
//
//*****************************************************************************

#ifndef __MOS_TraceEditor_h_
#define __MOS_TraceEditor_h_

#include "MOS_Types.h"

#include <QFrame.h>
#include <QWidget.h>

class MOS_TraceListView;
class QWidget;
class MOS_Agent;
class MOS_AttrEditor;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_TraceEditor : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_TraceEditor );


public:
    MOS_TraceEditor( MOS_AttrEditor& attrEditor,QWidget* pParent = 0 );
    ~MOS_TraceEditor();

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
    MOS_TraceListView&      GetTraceListView () const;
    bool                    IsActive        ( bool bMove );
    void                    SetAgent        ( MOS_Agent* pAgent );
    void                    SetWidgetTab    ( QWidget* pWidget );
    //@}

private:
    MOS_TraceListView*      pTraceListView_;
    MOS_AttrEditor&         attrEditor_;
    QWidget*                pWidgetTab_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_TraceEditor.inl"
#endif


#endif // __MOS_TraceEditor_h_