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

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Team;
class MOS_ObjectKnowledge;
class MOS_ActionContext;
class MOS_SelectedElement;
class QCheckBox;

// =============================================================================
/** @class  MOS_ObjectKnowledgePanel
    @brief  MOS_ObjectKnowledgePanel
    @par    Using example
    @code
    MOS_ObjectKnowledgePanel;
    @endcode
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
    MOS_ObjectKnowledgePanel( QWidget* pParent );
    ~MOS_ObjectKnowledgePanel();
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

    void OnKnowledgeCreated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void OnKnowledgeUpdated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void OnKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
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

    MOS_ObjectKnowledge* pSelectedKnowledge_;

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

    // Crossing
    QGroupBox* pCrossingGroup_;
    QLabel*    pCrossingWidthLabel_;
    QLabel*    pCrossingDepthLabel_;
    QLabel*    pCrossingRiverSpeedLabel_;
    QLabel*    pCrossingBanksNeedWorkLabel_;

    // NBC
    QGroupBox* pNBCGroup_;
    QLabel*    pNBCAgentLabel_;

    // ROTA
    QGroupBox* pROTAGroup_;
    QLabel*    pROTADangerLabel_;
    QLabel*    pROTANBCAgentsLabel_;

    // Camp
    QGroupBox* pCampGroup_;
    QLabel*    pCampTC2ID_;

    // Logistic route
    QGroupBox* pLogRouteGroup_;
    QLabel*    pLogRouteWidth_;
    QLabel*    pLogRouteLength_;
    QLabel*    pLogRouteFlow_;
    QLabel*    pLogRouteMaxWeight_;
    QLabel*    pLogRouteEquipped_;

    QListView* pPerceptionListView_;
    QPopupMenu* pPopupMenu_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ObjectKnowledgePanel.inl"
#endif

#endif // __MOS_ObjectKnowledgePanel_h_
