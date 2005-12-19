// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SelectedElement.inl $
// $Author: Ape $
// $Modtime: 21/06/04 15:31 $
// $Revision: 5 $
// $Workfile: MOS_SelectedElement.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement::IsAMapElementSelected
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
bool MOS_SelectedElement::IsAMapElementSelected() const
{
    return pAgent_ != 0 || pObject_ != 0 || pLine_ != 0 || pAgentKnowledge_ != 0 
            || pObjectKnowledge_ != 0 || pPopulationConcentration_ != 0 
            || pPopulationConcentrationKnowledge_ != 0 || pPopulationFlow_ != 0 
            || pPopulationFlowKnowledge_ != 0 || pPopulation_ != 0 || pPopulationKnowledge_ != 0;
}
