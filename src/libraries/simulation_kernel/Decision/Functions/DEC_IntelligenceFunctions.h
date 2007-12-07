// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_IntelligenceFunctions_h_
#define __DEC_IntelligenceFunctions_h_

class MIL_Automate;

// =============================================================================
/** @class  DEC_IntelligenceFunctions
    @brief  DEC_IntelligenceFunctions
*/
// Created: SBO 2007-11-12
// =============================================================================
class DEC_IntelligenceFunctions
{

public:
    //! @name Operations
    //@{
    static void ComputeUnloadedEnemiesRatio   ( DIA_Call_ABC& call, const MIL_Automate& caller );
    static void ComputeLoadedEnemiesRatio     ( DIA_Call_ABC& call, const MIL_Automate& caller );
    static void SortAccordingToUnloadedEnemies( DIA_Call_ABC& call, const MIL_Automate& caller );
    static void SortAccordingToLoadedEnemies  ( DIA_Call_ABC& call, const MIL_Automate& caller );
    static void IsEnemyOnFlank                ( DIA_Call_ABC& call, const MIL_Automate& caller );
    static void IsFriendOnFlank               ( DIA_Call_ABC& call, const MIL_Automate& caller );
    static void ComputeCoverDirection         ( DIA_Call_ABC& call, const MIL_Automate& caller );
    //@}
};

#endif // __DEC_IntelligenceFunctions_h_
