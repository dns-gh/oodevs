// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledgePanel.h $
// $Author: Age $
// $Modtime: 5/04/05 18:33 $
// $Revision: 6 $
// $Workfile: ObjectKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __ObjectKnowledgePanel_h_
#define __ObjectKnowledgePanel_h_

#include "Types.h"
#include "InfoPanel_ABC.h"

class Team;
class ObjectKnowledge;
class ActionContext;
class SelectedElement;
class QCheckBox;
class Display;

// =============================================================================
/** @class  ObjectKnowledgePanel
    @brief  ObjectKnowledgePanel
*/
// Created: APE 2004-05-04
// =============================================================================
class ObjectKnowledgePanel : public InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ObjectKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectKnowledgePanel( QWidget* pParent );
    virtual ~ObjectKnowledgePanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
            void UpdateList      ();
            void UpdateSelected  ();
    Team* GetSelectedTeam( const SelectedElement& item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestCenter();

    void OnKnowledgeCreated( Team& team, ObjectKnowledge& knowledge );
    void OnKnowledgeUpdated( Team& team, ObjectKnowledge& knowledge );
    void OnKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge );
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );

    void ToggleDisplayOwnTeam();
    //@}

signals:
    //! @name Signals
    //@{
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& action );
    void ElementSelected( SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    //@}

private:
    //! @name Member data
    //@{
    Team* pTeam_;
    QListView* pKnowledgeListView_;
    QCheckBox* pOwnTeamCheckBox_;

    ObjectKnowledge* pSelectedKnowledge_;

    Display* display_;

    QListView* pPerceptionListView_;
    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __ObjectKnowledgePanel_h_
