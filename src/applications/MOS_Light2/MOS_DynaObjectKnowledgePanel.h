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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectKnowledgePanel.h $
// $Author: Age $
// $Modtime: 5/04/05 18:33 $
// $Revision: 6 $
// $Workfile: MOS_DynaObjectKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __MOS_DynaObjectKnowledgePanel_h_
#define __MOS_DynaObjectKnowledgePanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Team;
class MOS_DynaObjectKnowledge;
class MOS_ActionContext;
class MOS_SelectedElement;
class QCheckBox;

// =============================================================================
/** @class  MOS_DynaObjectKnowledgePanel
    @brief  MOS_DynaObjectKnowledgePanel
    @par    Using example
    @code
    MOS_DynaObjectKnowledgePanel;
    @endcode
*/
// Created: APE 2004-05-04
// =============================================================================
class MOS_DynaObjectKnowledgePanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_DynaObjectKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_DynaObjectKnowledgePanel( QWidget* pParent );
    ~MOS_DynaObjectKnowledgePanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnUpdate();
    void UpdateList();
    void UpdateSelected();
    MOS_Team* GetSelectedTeam( const MOS_SelectedElement& item );
    //@}

private slots:
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestCenter();

    void OnKnowledgeCreated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void OnKnowledgeUpdated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void OnKnowledgeDeleted( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );

    void ToggleDisplayOwnTeam();

signals:
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& action );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );

private:
    //! @name Member data
    //@{
    MOS_Team* pTeam_;
    QListView* pKnowledgeListView_;
    QCheckBox* pOwnTeamCheckBox_;

    MOS_DynaObjectKnowledge* pSelectedKnowledge_;

    QLabel* pIdLabel_;
    QLabel* pAssociatedObjectLabel_;
    QLabel* pPositionLabel_;
    QLabel* pObjectTypeLabel_;
    QLabel* pPercentBuiltLabel_;
    QLabel* pPercentValueLabel_;
    QLabel* pPercentAroundLabel_;
    QLabel* pIsUnderPrepLabel_;
    QLabel* pIsPreceivedLabel_;
    QLabel* pRevelanceLabel_;

    QLabel* pCrossingWidthTitle_;
    QLabel* pCrossingWidthLabel_;
    QLabel* pCrossingDepthTitle_;
    QLabel* pCrossingDepthLabel_;
    QLabel* pCrossingRiverSpeedTitle_;
    QLabel* pCrossingRiverSpeedLabel_;
    QLabel* pCrossingBanksNeedWorkTitle_;
    QLabel* pCrossingBanksNeedWorkLabel_;

    QLabel* pNBCAgentTitle_;
    QLabel* pNBCAgentLabel_;

    QListView* pPerceptionListView_;

    QPopupMenu* pPopupMenu_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_DynaObjectKnowledgePanel.inl"
#endif

#endif // __MOS_DynaObjectKnowledgePanel_h_
