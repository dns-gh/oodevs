// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPanel_h_
#define __PopulationPanel_h_

#include "InfoPanel_ABC.h"

class QListViewItem;
class Display;

// =============================================================================
// Created: HME 2005-10-03
// =============================================================================
class PopulationPanel : public InfoPanel_ABC
{
    Q_OBJECT

    MT_COPYNOTALLOWED( PopulationPanel );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationPanel( QWidget* pParent );
    virtual ~PopulationPanel();
    //@}

signals:
    //! @name Signals
    //@{
    void ElementSelected( SelectedElement& selectedElement );
    //@}

private:
    //! @name Event handlers
    //@{
	virtual void OnClearSelection();
    virtual void OnUpdate();
	virtual void OnPopulationUpdated( const Population& population );
    //@}

    //! @name Helpers
    //@{
    void UpdateSelected();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange               ( QListViewItem* pItem );
    void OnPopulationConcentrationCreated( const PopulationConcentration& concentration );
    void OnPopulationConcentrationUpdated( const PopulationConcentration& concentration );
    void OnPopulationConcentrationDeleted( const PopulationConcentration& concentration );
    void OnPopulationFlowCreated         ( const PopulationFlow&          flow );
    void OnPopulationFlowUpdated         ( const PopulationFlow&          flow );
    void OnPopulationFlowDeleted         ( const PopulationFlow&          flow );
    //@}

private:
    //! @name Types
    //@{
    enum E_ItemType
    {
        eConcentration,
        eFlow
    };

    typedef MT_ValuedListViewItem< const PopulationConcentration*, eConcentration > T_PopulationConcentrationItem;
    typedef MT_ValuedListViewItem< const PopulationFlow*         , eFlow >          T_PopulationFlowItem;
    //@}

private:
    const Population* pPopulation_;
    Display* display_;
    QListView* pPartsListView_;
};

#endif // __PopulationPanel_h_
