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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Nature.h $
// $Author: Age $
// $Modtime: 15/04/05 15:38 $
// $Revision: 3 $
// $Workfile: MOS_Nature.h $
//
// *****************************************************************************

#ifndef __MOS_Nature_h_
#define __MOS_Nature_h_

// =============================================================================
/** @class  MOS_Nature
    @brief  MOS_Nature
*/
// Created: AGE 2005-04-07
// =============================================================================
class MOS_Nature
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Nature( MT_InputArchive_ABC& archive );
    virtual ~MOS_Nature();
    //@}
private:
    //! @name Helpers
    //@{
    template< typename Enum, typename Solver >
    void Read( MT_InputArchive_ABC& archive, const std::string& field, const Solver& solver, Enum& result );

    static char Symbol( const MOS_Nature& n );
    static char LevelSymbol( const MOS_Nature& n );

    static char CombatSymbol( const MOS_Nature& n );
    static char LogSymbol( const MOS_Nature& n );
    static char SupportSymbol( const MOS_Nature& n );

    static char CombatAirDefenseSymbol( const MOS_Nature& n );
    static char CombatSecuritySymbol( const MOS_Nature& n );
    static char CombatArmoredSymbol( const MOS_Nature& n );
    static char CombatAntiCharSymbol( const MOS_Nature& n );
    static char CombatAviationSymbol( const MOS_Nature& n );
    static char CombatInfantrySymbol( const MOS_Nature& n );
    static char CombatGenieSymbol( const MOS_Nature& n );
    static char CombatArtillerySymbol( const MOS_Nature& n );
    static char CombatReconSymbol( const MOS_Nature& n );
    static char CombatMissileSymbol( const MOS_Nature& n );

    static char LogAdministrativeSymbol( const MOS_Nature& n );
    static char LogMedicalSymbol( const MOS_Nature& n );
    static char LogSupplySymbol( const MOS_Nature& n );
    static char LogTransportSymbol( const MOS_Nature& n );
    static char LogReparationSymbol( const MOS_Nature& n );

    static char SupportNbcSymbol( const MOS_Nature& n );
    static char SupportRensSymbol( const MOS_Nature& n );
    static char SupportLawSymbol( const MOS_Nature& n );
    static char SupportTransSymbol( const MOS_Nature& n );
    static char SupportInfWarSymbol( const MOS_Nature& n );
    static char SupportLandingSymbol( const MOS_Nature& n );
    static char SupportMineSweepSymbol( const MOS_Nature& n );
    static char SupportManoeuvreSymbol( const MOS_Nature& n );

    static char CombatGenieCombatSymbol( const MOS_Nature& n );
    static char CombatGenieConstructionSymbol( const MOS_Nature& n );
    static char SupportElecWarSymbol( const MOS_Nature& n );


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

#endif // __MOS_Nature_h_
