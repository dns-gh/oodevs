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
class MIL_Fuseau;
class TER_Localisation;

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
    static float ComputeUnloadedEnemiesRatio      ( const MIL_Automate& caller, const TER_Localisation* location );
    static float ComputeLoadedEnemiesRatio        ( const MIL_Automate& caller, const TER_Localisation* location );
    static float ComputeFuseauUnloadedEnemiesRatio( const MIL_Automate& caller, const MIL_Fuseau* limits );
    static float ComputeFuseauLoadedEnemiesRatio  ( const MIL_Automate& caller, const MIL_Fuseau* limits );
    static std::vector< boost::shared_ptr< TER_Localisation > > SortZonesAccordingToUnloadedEnemies( const MIL_Automate& caller, const std::vector< boost::shared_ptr< TER_Localisation > >& locations );
    static std::vector< boost::shared_ptr< MIL_Fuseau > > SortAccordingToUnloadedEnemies   ( const MIL_Automate& caller, const std::vector< boost::shared_ptr< MIL_Fuseau > >& limits );
    static std::vector< boost::shared_ptr< MIL_Fuseau > > SortAccordingToLoadedEnemies     ( const MIL_Automate& caller, const std::vector< boost::shared_ptr< MIL_Fuseau > >& limits );
    static bool IsFriendOnFlank( const MIL_Automate& caller, const MIL_Fuseau* limits );
    static boost::shared_ptr< MT_Vector2D > ComputeCoverDirection( const MIL_Automate& caller, const MT_Vector2D* origin, const MIL_Fuseau* limits );
    //@}
};

#endif // __DEC_IntelligenceFunctions_h_
