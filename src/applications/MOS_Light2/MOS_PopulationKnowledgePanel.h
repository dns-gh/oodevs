// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-10-19 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationKnowledgePanel.h $
// $Author: Sbo $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: MOS_PopulationKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __MOS_PopulationKnowledgePanel_h_
#define __MOS_PopulationKnowledgePanel_h_

#include "MOS_Types.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Gtia;
class MOS_PopulationKnowledge;
class MOS_PopulationConcentrationKnowledge;
class MOS_PopulationFlowKnowledge;
class MOS_ActionContext;
class MOS_SelectedElement;
class QCheckBox;
class QTable;
class MOS_Display;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class MOS_PopulationKnowledgePanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_PopulationKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_PopulationKnowledgePanel( QWidget* pParent );
    virtual ~MOS_PopulationKnowledgePanel();
    //@}

protected:
    //! @name Update/Clean display
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
    //@}

private:
    //! @name Helpers
    //@{
    void      UpdateList     ();
    void      UpdateSelected ();
    MOS_Gtia* GetSelectedGtia();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged             ( QListViewItem* pItem );
    void OnRequestCenter                ();

    void OnKnowledgeCreated             ( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    void OnKnowledgeUpdated             ( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    void OnKnowledgeDeleted             ( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );

    void OnConcentrationKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );
    void OnConcentrationKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );
    void OnConcentrationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );

    void OnFlowKnowledgeCreated         ( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );
    void OnFlowKnowledgeUpdated         ( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );
    void OnFlowKnowledgeDeleted         ( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );

    void OnContextMenuRequested         ( QListViewItem* pItem, const QPoint&            pos       );
    void ToggleDisplayOwnTeam           ();
    //@}

signals:
    //! @name Signals
    //@{
    void NewPopupMenu   ( QPopupMenu&          popupMenu, const MOS_ActionContext& action );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void CenterOnPoint  ( const MT_Vector2D&   vPoint );
    //@}

private:
    //! @name Enums
    //@{
    enum E_ItemType
    {
        ePopulation,
        eConcentration,
        eFlow
    };
    //@}

private:
    //! @name Member data
    //@{
    MOS_Gtia*                             pGtia_;
    MOS_PopulationKnowledge*              pSelectedKnowledge_;
    MOS_PopulationConcentrationKnowledge* pSelectedConcentrationKnowledge_;
    MOS_PopulationFlowKnowledge*          pSelectedFlowKnowledge_;

    QListView* pKnowledgeListView_;
    QListView* pPerceptionListView_;
    QCheckBox* pOwnTeamCheckBox_;
    
    MOS_Display* display_;

    QGroupBox* pFlowPartBox_;
    QLabel*    pFlowPartNbr_;
    QTable*    pFlowPartTable_;

    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __MOS_PopulationKnowledgePanel_h_
