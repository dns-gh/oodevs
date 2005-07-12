// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationMapFilter.h $
// $Author: Ape $
// $Modtime: 5/10/04 16:00 $
// $Revision: 3 $
// $Workfile: MOS_AgentCreationMapFilter.h $
//
// *****************************************************************************

#ifndef __MOS_AgentCreationMapFilter_h_
#define __MOS_AgentCreationMapFilter_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MapEventFilter_ABC.h"
#include "MOS_AgentCreationPanel.h"
#include "MOS_SelectedElement.h"

class MOS_MapDropEvent;
class MOS_MapMouseEvent;

// =============================================================================
/** @class  MOS_AgentCreationMapFilter
    @brief  MOS_AgentCreationMapFilter
*/
// Created: APE 2004-09-02
// =============================================================================
class MOS_AgentCreationMapFilter : public QObject, public MOS_MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_AgentCreationMapFilter );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_AgentCreationMapFilter( MOS_AgentCreationPanel& creationPanel );
    ~MOS_AgentCreationMapFilter();
    //@}

    //! @name Accessors
    //@{
    QObject* GetObject();
    //@}

private slots:
    //! @name Slots
    //@{
    void SetSelectedElement( MOS_SelectedElement& selectedElement );
    //@}

private:
    //! @name Event filter & related helpers
    //@{
    bool eventFilter( QObject* pSender, QEvent* pEvent );

    bool OnDragEnter ( QDragEnterEvent& dragEnterEvent );
    bool OnDrop      ( const MOS_MapDropEvent& mapDropEvent );
    bool OnMousePress( const MOS_MapMouseEvent& mapMouseEvent );
    bool OnMouseMove ( const MOS_MapMouseEvent& mapMouseEvent );
    //@}

    //! @name Helpers
    //@{
    MOS_Agent& CreateAgent( const MOS_AgentCreationPanel::MOS_AgentType& agentType, const MT_Vector2D& vPos, MOS_Gtia& gtia, MOS_Agent* pParent );
    //@}

private:
    //! @name Member data
    //@{
    MOS_AgentCreationPanel& creationPanel_;
    MOS_SelectedElement selectedElement_;
    MT_Vector2D vOldMousePos_;
    //@}
};

#include "MOS_AgentCreationMapFilter.inl"

#endif // __MOS_AgentCreationMapFilter_h_
