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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationPanel.inl $
// $Author: Ape $
// $Modtime: 23/09/04 10:09 $
// $Revision: 3 $
// $Workfile: MOS_AgentCreationPanel.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::GetSelectedElement
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
inline
MOS_SelectedElement& MOS_AgentCreationPanel::GetSelectedElement()
{
    return selectedElement_;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::GetDispersionRadius
/** @return 
*/
// Created: APE 2004-09-23
// -----------------------------------------------------------------------------
inline
float MOS_AgentCreationPanel::GetDispersionRadius() const
{
    return pRadiusSpinBox_->value();
}

