// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationPanel_h_
#define __MOS_PopulationPanel_h_

#include "MOS_InfoPanel_ABC.h"

class QListViewItem;

// =============================================================================
// Created: HME 2005-10-03
// =============================================================================
class MOS_PopulationPanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT

    MT_COPYNOTALLOWED( MOS_PopulationPanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_PopulationPanel( QWidget* pParent );
    virtual ~MOS_PopulationPanel();
    //@}

signals:
    //! @name Signals
    //@{
    void ElementSelected( MOS_SelectedElement& selectedElement );
    //@}

private:
    //! @name Event handlers
    //@{
	virtual void OnClearSelection();
    virtual void OnUpdate();
	virtual void OnPopulationUpdated( const MOS_Population& population );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* pItem );
    //@}

private:
    //! @name Types
    //@{
    enum E_ItemType
    {
        eConcentration,
        eFlow
    };
    //@}

private:
    QLabel*    pNameLabel_;
    QLabel*    pLivingLabel_;
    QLabel*    pDeadLabel_;
	QListView* pPartsListView_;
};

#endif // __MOS_PopulationPanel_h_
