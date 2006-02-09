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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectKnowledgePanel.h $
// $Author: Age $
// $Modtime: 5/04/05 18:33 $
// $Revision: 6 $
// $Workfile: MOS_ObjectKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __MOS_ObjectKnowledgePanel_h_
#define __MOS_ObjectKnowledgePanel_h_

#include "MOS_Types.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Team;
class MOS_ObjectKnowledge;
class MOS_ActionContext;
class MOS_SelectedElement;
class QCheckBox;
class MOS_Display;

// =============================================================================
/** @class  MOS_ObjectKnowledgePanel
    @brief  MOS_ObjectKnowledgePanel
*/
// Created: APE 2004-05-04
// =============================================================================
class MOS_ObjectKnowledgePanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ObjectKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_ObjectKnowledgePanel( QWidget* pParent );
    virtual ~MOS_ObjectKnowledgePanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
            void UpdateList      ();
            void UpdateSelected  ();
    MOS_Team* GetSelectedTeam( const MOS_SelectedElement& item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestCenter();

    void OnKnowledgeCreated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void OnKnowledgeUpdated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void OnKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );

    void ToggleDisplayOwnTeam();
    //@}

signals:
    //! @name Signals
    //@{
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& action );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    //@}

private:
    //! @name Member data
    //@{
    MOS_Team* pTeam_;
    QListView* pKnowledgeListView_;
    QCheckBox* pOwnTeamCheckBox_;

    MOS_ObjectKnowledge* pSelectedKnowledge_;

    MOS_Display* display_;

    QListView* pPerceptionListView_;
    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __MOS_ObjectKnowledgePanel_h_
