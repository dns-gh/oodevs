// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledgePanel.h $
// $Author: Age $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: AgentKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __AgentKnowledgePanel_h_
#define __AgentKnowledgePanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "InfoPanel_ABC.h"

class Gtia;
class AgentKnowledge;
class ActionContext;
class SelectedElement;
class QCheckBox;
class Display;

// =============================================================================
/** @class  AgentKnowledgePanel
    @brief  AgentKnowledgePanel
*/
// Created: APE 2004-05-03
// =============================================================================
class AgentKnowledgePanel : public InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( AgentKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgePanel( QWidget* pParent );
    virtual ~AgentKnowledgePanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void      OnUpdate        ();
    virtual void      OnClearSelection();
            void      UpdateList      ();
            void      UpdateSelected  ();
            Gtia* GetSelectedGtia ( const SelectedElement& item );
    //@}

private slots:
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestCenter();

    void OnKnowledgeCreated( Gtia& gtia, AgentKnowledge& knowledge );
    void OnKnowledgeUpdated( Gtia& gtia, AgentKnowledge& knowledge );
    void OnKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge );
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );

    void ToggleDisplayOwnTeam();

signals:
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& action );
    void ElementSelected( SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );

private:
    //! @name Member data
    //@{
    Gtia* pGtia_;
    QListView* pKnowledgeListView_;
    QCheckBox* pOwnTeamCheckBox_;
    AgentKnowledge* pSelectedKnowledge_;

    Display* display_;

    QListView* pPerceptionListView_;

    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __AgentKnowledgePanel_h_
