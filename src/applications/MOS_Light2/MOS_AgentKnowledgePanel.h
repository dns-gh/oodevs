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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledgePanel.h $
// $Author: Age $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: MOS_AgentKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentKnowledgePanel_h_
#define __MOS_AgentKnowledgePanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Gtia;
class MOS_AgentKnowledge;
class MOS_ActionContext;
class MOS_SelectedElement;
class QCheckBox;
class MOS_Display;

// =============================================================================
/** @class  MOS_AgentKnowledgePanel
    @brief  MOS_AgentKnowledgePanel
*/
// Created: APE 2004-05-03
// =============================================================================
class MOS_AgentKnowledgePanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_AgentKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_AgentKnowledgePanel( QWidget* pParent );
    virtual ~MOS_AgentKnowledgePanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void      OnUpdate        ();
    virtual void      OnClearSelection();
            void      UpdateList      ();
            void      UpdateSelected  ();
            MOS_Gtia* GetSelectedGtia ( const MOS_SelectedElement& item );
    //@}

private slots:
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestCenter();

    void OnKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void OnKnowledgeUpdated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void OnKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );

    void ToggleDisplayOwnTeam();

signals:
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& action );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );

private:
    //! @name Member data
    //@{
    MOS_Gtia* pGtia_;
    QListView* pKnowledgeListView_;
    QCheckBox* pOwnTeamCheckBox_;
    MOS_AgentKnowledge* pSelectedKnowledge_;

    MOS_Display* display_;

    QListView* pPerceptionListView_;

    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __MOS_AgentKnowledgePanel_h_
