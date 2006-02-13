// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-07 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Nature.h $
// $Author: Age $
// $Modtime: 15/04/05 15:38 $
// $Revision: 3 $
// $Workfile: Nature.h $
//
// *****************************************************************************

#ifndef __Nature_h_
#define __Nature_h_

#include "Types.h"

// =============================================================================
/** @class  Nature
    @brief  Nature
*/
// Created: AGE 2005-04-07
// =============================================================================
class Nature
{

public:
    //! @name Constructors/Destructor
    //@{
             Nature( InputArchive& archive );
    virtual ~Nature();
    //@}
private:
    //! @name Helpers
    //@{
    template< typename Enum, typename Solver >
    void Read( InputArchive& archive, const std::string& field, const Solver& solver, Enum& result );

    static char Symbol( const Nature& n );
    static char LevelSymbol( const Nature& n );

    static char CombatSymbol( const Nature& n );
    static char LogSymbol( const Nature& n );
    static char SupportSymbol( const Nature& n );

    static char CombatAirDefenseSymbol( const Nature& n );
    static char CombatSecuritySymbol( const Nature& n );
    static char CombatArmoredSymbol( const Nature& n );
    static char CombatAntiCharSymbol( const Nature& n );
    static char CombatAviationSymbol( const Nature& n );
    static char CombatInfantrySymbol( const Nature& n );
    static char CombatGenieSymbol( const Nature& n );
    static char CombatArtillerySymbol( const Nature& n );
    static char CombatReconSymbol( const Nature& n );
    static char CombatMissileSymbol( const Nature& n );

    static char LogAdministrativeSymbol( const Nature& n );
    static char LogMedicalSymbol( const Nature& n );
    static char LogSupplySymbol( const Nature& n );
    static char LogTransportSymbol( const Nature& n );
    static char LogReparationSymbol( const Nature& n );

    static char SupportNbcSymbol( const Nature& n );
    static char SupportRensSymbol( const Nature& n );
    static char SupportLawSymbol( const Nature& n );
    static char SupportTransSymbol( const Nature& n );
    static char SupportInfWarSymbol( const Nature& n );
    static char SupportLandingSymbol( const Nature& n );
    static char SupportMineSweepSymbol( const Nature& n );
    static char SupportManoeuvreSymbol( const Nature& n );

    static char CombatGenieCombatSymbol( const Nature& n );
    static char CombatGenieConstructionSymbol( const Nature& n );
    static char SupportElecWarSymbol( const Nature& n );


    static unsigned int ConvertToNatureLevel( const std::string& strName );
    static unsigned int ConvertToUnitNatureWeapon( const std::string& strName );
    static unsigned int ConvertToUnitNatureSpecialization( const std::string& strName );
    static unsigned int ConvertToUnitNatureQualifier( const std::string& strName );
    static unsigned int ConvertToUnitNatureCategory( const std::string& strName );
    static unsigned int ConvertToUnitNatureMobility( const std::string& strName );
    static unsigned int ConvertToUnitCapaciteMission( const std::string& strName );

    //@}

public:
    //! @name Member data
    //@{
    E_NatureLevel              level_;
    E_UnitNatureWeapon         weapon_;
    E_UnitNatureSpecialization specialization_;
    E_UnitNatureQualifier      qualifier_;
    E_UnitNatureCategory       category_;
    E_UnitNatureMobility       mobility_;
    E_UnitCapaciteMission      capacity_;

    char levelSymbol_;
    char symbol_;
    //@}
};

#endif // __Nature_h_
