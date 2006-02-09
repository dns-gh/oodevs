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
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationKnowledgePanel.h $
// $Author: Sbo $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: PopulationKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __PopulationKnowledgePanel_h_
#define __PopulationKnowledgePanel_h_

#include "Types.h"
#include "InfoPanel_ABC.h"

class Gtia;
class PopulationKnowledge;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class ActionContext;
class SelectedElement;
class QCheckBox;
class QTable;
class Display;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class PopulationKnowledgePanel : public InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( PopulationKnowledgePanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationKnowledgePanel( QWidget* pParent );
    virtual ~PopulationKnowledgePanel();
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
    Gtia* GetSelectedGtia();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged             ( QListViewItem* pItem );
    void OnRequestCenter                ();

    void OnKnowledgeCreated             ( Gtia& gtia, PopulationKnowledge& knowledge );
    void OnKnowledgeUpdated             ( Gtia& gtia, PopulationKnowledge& knowledge );
    void OnKnowledgeDeleted             ( Gtia& gtia, PopulationKnowledge& knowledge );

    void OnConcentrationKnowledgeCreated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );
    void OnConcentrationKnowledgeUpdated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );
    void OnConcentrationKnowledgeDeleted( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );

    void OnFlowKnowledgeCreated         ( Gtia& gtia, PopulationFlowKnowledge& knowledge );
    void OnFlowKnowledgeUpdated         ( Gtia& gtia, PopulationFlowKnowledge& knowledge );
    void OnFlowKnowledgeDeleted         ( Gtia& gtia, PopulationFlowKnowledge& knowledge );

    void OnContextMenuRequested         ( QListViewItem* pItem, const QPoint&            pos       );
    void ToggleDisplayOwnTeam           ();
    //@}

signals:
    //! @name Signals
    //@{
    void NewPopupMenu   ( QPopupMenu&          popupMenu, const ActionContext& action );
    void ElementSelected( SelectedElement& selectedElement );
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
    Gtia*                             pGtia_;
    PopulationKnowledge*              pSelectedKnowledge_;
    PopulationConcentrationKnowledge* pSelectedConcentrationKnowledge_;
    PopulationFlowKnowledge*          pSelectedFlowKnowledge_;

    QListView* pKnowledgeListView_;
    QListView* pPerceptionListView_;
    QCheckBox* pOwnTeamCheckBox_;
    
    Display* display_;

    QGroupBox* pFlowPartBox_;
    QLabel*    pFlowPartNbr_;
    QTable*    pFlowPartTable_;

    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __PopulationKnowledgePanel_h_
