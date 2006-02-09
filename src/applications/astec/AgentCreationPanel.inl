// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-26 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationPanel.inl $
// $Author: Ape $
// $Modtime: 23/09/04 10:09 $
// $Revision: 3 $
// $Workfile: AgentCreationPanel.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::GetSelectedElement
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
inline
SelectedElement& AgentCreationPanel::GetSelectedElement()
{
    return selectedElement_;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::GetDispersionRadius
/** @return 
*/
// Created: APE 2004-09-23
// -----------------------------------------------------------------------------
inline
float AgentCreationPanel::GetDispersionRadius() const
{
    return pRadiusSpinBox_->value();
}

