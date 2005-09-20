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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentListView.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:42 $
// $Revision: 9 $
// $Workfile: MOS_AgentListView.h $
//
// *****************************************************************************

#ifndef __MOS_AgentListView_h_
#define __MOS_AgentListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_Team;
class MOS_Gtia;
class MOS_Agent;
class MOS_ActionContext;
class MOS_SelectedElement;


// =============================================================================
/** @class  MOS_AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class MOS_AgentListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_AgentListView );

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
     MOS_AgentListView( QWidget* pParent );
    ~MOS_AgentListView();

    QSize sizeHint() const;
    //@}

public slots:
    void AddAgent( MOS_Agent& agent );
    void AddGtia( MOS_Gtia& gtia );
    void SetSelectedElement( MOS_SelectedElement& selectedElement );
    void OnAgentUpdated( MOS_Agent& agent );

private slots:
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnRequestCenter();

    void OnSelectionChange( QListViewItem* pItem );
    void OnTeamChanged();
    void OnAgentReparented( MOS_Agent& agent );

private:
    QDragObject* dragObject();
    void dropEvent( QDropEvent* pEvent );
    void dragEnterEvent( QDragEnterEvent* pEvent );

    void keyPressEvent( QKeyEvent* pEvent );

signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );

private:
    //! @name Helpers
    //@{
    MOS_Team*  ToTeam ( QListViewItem* pItem );
    MOS_Gtia*  ToGtia ( QListViewItem* pItem );
    MOS_Agent* ToAgent( QListViewItem* pItem );

    void DeleteAssociatedElements( QListViewItem* pItem );
    //@}

private:
    QPopupMenu* pPopupMenu_;
};

#   include "MOS_AgentListView.inl"

#endif // __MOS_AgentListView_h_
