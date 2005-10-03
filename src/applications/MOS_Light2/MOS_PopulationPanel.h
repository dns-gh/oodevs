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

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_InfoPanel_ABC.h"

// =============================================================================
/** @class  MOS_PopulationPanel
    @brief  MOS_PopulationPanel
    @par    Using example
    @code
    MOS_PopulationPanel;
    @endcode
*/
// Created: HME 2005-10-03
// =============================================================================
class MOS_PopulationPanel : public MOS_InfoPanel_ABC
{
    MT_COPYNOTALLOWED( MOS_PopulationPanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_PopulationPanel(  QWidget* pParent );
    virtual ~MOS_PopulationPanel();

	virtual void OnClearSelection();

private slots:

    virtual void OnUpdate();
	virtual void OnPopulationUpdated( MOS_Population& population );


private:
    QLabel*    pNameLabel_;
    QLabel*    pLivingLabel_;
    QLabel*    pDeadLabel_;
    QLabel*    pAttitudeLabel_;
};



#include "MOS_PopulationPanel.inl"

#endif // __MOS_PopulationPanel_h_
