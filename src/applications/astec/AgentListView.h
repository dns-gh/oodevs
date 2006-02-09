// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentListView.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:42 $
// $Revision: 9 $
// $Workfile: AgentListView.h $
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

class Team;
class Gtia;
class Agent;
class ActionContext;
class SelectedElement;


// =============================================================================
/** @class  AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class AgentListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( AgentListView );

private:
    enum
    {
        eTeam  = 1000,
        eGtia  = 1001,
        eAgent = 1002
    };
    
public:
    //! @name Constructors/Destructor
    //@{
     AgentListView( QWidget* pParent );
    ~AgentListView();

    QSize sizeHint() const;
    //@}

public slots:
    void AddAgent( Agent& agent );
    void AddGtia( Gtia& gtia );
    void SetSelectedElement( SelectedElement& selectedElement );
    void OnAgentUpdated( Agent& agent );

private slots:
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnRequestCenter();

    void OnSelectionChange( QListViewItem* pItem );
    void OnTeamChanged();
    void OnAgentReparented( Agent& agent );

private:
    QDragObject* dragObject();
    void dropEvent( QDropEvent* pEvent );
    void dragEnterEvent( QDragEnterEvent* pEvent );

    void keyPressEvent( QKeyEvent* pEvent );

signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void ElementSelected( SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& context );

private:
    //! @name Helpers
    //@{
    Team*  ToTeam ( QListViewItem* pItem );
    Gtia*  ToGtia ( QListViewItem* pItem );
    Agent* ToAgent( QListViewItem* pItem );

    void DeleteAssociatedElements( QListViewItem* pItem );
    //@}

private:
    QPopupMenu* pPopupMenu_;
};

#   include "AgentListView.inl"

#endif // __AgentListView_h_
