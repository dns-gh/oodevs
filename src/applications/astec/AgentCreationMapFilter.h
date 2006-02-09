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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationMapFilter.h $
// $Author: Ape $
// $Modtime: 5/10/04 16:00 $
// $Revision: 3 $
// $Workfile: AgentCreationMapFilter.h $
//
// *****************************************************************************

#ifndef __AgentCreationMapFilter_h_
#define __AgentCreationMapFilter_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MapEventFilter_ABC.h"
#include "AgentCreationPanel.h"
#include "SelectedElement.h"

class MapDropEvent;
class MapMouseEvent;

// =============================================================================
/** @class  AgentCreationMapFilter
    @brief  AgentCreationMapFilter
*/
// Created: APE 2004-09-02
// =============================================================================
class AgentCreationMapFilter : public QObject, public MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( AgentCreationMapFilter );

public:
    //! @name Constructors/Destructor
    //@{
     AgentCreationMapFilter( AgentCreationPanel& creationPanel );
    ~AgentCreationMapFilter();
    //@}

    //! @name Accessors
    //@{
    QObject* GetObject();
    //@}

private slots:
    //! @name Slots
    //@{
    void SetSelectedElement( SelectedElement& selectedElement );
    //@}

private:
    //! @name Event filter & related helpers
    //@{
    bool eventFilter( QObject* pSender, QEvent* pEvent );

    bool OnDragEnter ( QDragEnterEvent& dragEnterEvent );
    bool OnDrop      ( const MapDropEvent& mapDropEvent );
    bool OnMousePress( const MapMouseEvent& mapMouseEvent );
    bool OnMouseMove ( const MapMouseEvent& mapMouseEvent );
    //@}

    //! @name Helpers
    //@{
    Agent& CreateAgent( const AgentCreationPanel::AgentType& agentType, const MT_Vector2D& vPos, Gtia& gtia, Agent* pParent );
    //@}

private:
    //! @name Member data
    //@{
    AgentCreationPanel& creationPanel_;
    SelectedElement selectedElement_;
    MT_Vector2D vOldMousePos_;
    //@}
};

#include "AgentCreationMapFilter.inl"

#endif // __AgentCreationMapFilter_h_
