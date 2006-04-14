// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_KS_Perception.inl $
// $Author: Nld $
// $Modtime: 16/11/04 17:12 $
// $Revision: 3 $
// $Workfile: DEC_KS_Perception.inl $
//
// *****************************************************************************

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::MakePerceptionsAvailable
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
void DEC_KS_Perception::MakePerceptionsAvailable()
{
    bMakePerceptionsAvailable_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::MakePerceptionsAvailableTimed
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_Perception::MakePerceptionsAvailableTimed()
{
    bMakePerceptionsAvailableTimed_ = true;
}

