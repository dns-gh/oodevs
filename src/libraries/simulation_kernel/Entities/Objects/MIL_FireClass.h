// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FireClass_h_
#define __MIL_FireClass_h_

#include "MT_Tools/MT_Stl.h"

namespace xml
{
    class xistream;
}

class PHY_Weapon;
class PHY_DotationCategory;
class PHY_HumanWound;
class TerrainData;

namespace weather
{
    class PHY_Precipitation;
}

namespace urban
{
    class MaterialCompositionType;
}

// =============================================================================
// Created: RFT 22/04/2008
// MIL_FireClass.h
// Modified: RFT 19/05/2008
// =============================================================================
class MIL_FireClass : private boost::noncopyable
{
public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static const MIL_FireClass* Find( const std::string& strName );
    static const MIL_FireClass* GetDefaultFireClass();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    int GetInitialHeat() const;
    int GetDecreaseRate() const;
    int GetIncreaseRate() const;
    int GetMaxHeat() const;
    int GetWeatherDecreateRate( const weather::PHY_Precipitation& ) const;
    void GetSurfaceFirePotentials( const TerrainData& terrainData, int& ignitionThreshold, int& maxCombustionEnergy ) const;
    static int GetCellSize();
    //@}

    //! @name Operations
    //@{
    const PHY_HumanWound& ChooseRandomWound() const;
    //@}
private:
    //!@ Constructor and destructor
    //@{
     MIL_FireClass( const std::string& strName, xml::xistream& xis );
    ~MIL_FireClass();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_FireClass*, sCaseInsensitiveLess > T_FireClassMap;
    typedef T_FireClassMap::const_iterator                                    CIT_FireClassMap;

    struct ExtinguisherAgentEffect
    {
        int heatDecreaseRate_;
    };
    typedef std::map< const PHY_DotationCategory*, ExtinguisherAgentEffect > T_ExtinguisherAgentEffectMap;

    struct WeatherEffect
    {
        int heatDecreaseRate_;
    };
    typedef std::map< const weather::PHY_Precipitation*, WeatherEffect > T_WeatherEffectMap;

    struct Injury
    {
        double percentage_;
    };
    typedef std::map< const PHY_HumanWound*, Injury > T_InjuryMap;

    struct UrbanModifier
    {
        double factor_;
    };
    typedef std::map< const urban::MaterialCompositionType*, UrbanModifier > T_UrbanModifierMap;

    struct Surface
    {
        int ignitionThreshold_;
        int maxCombustionEnergy_;
    };
    typedef std::map< unsigned char/*terrain area*/, Surface > T_SurfaceMap;

private:
    //! @name Helpers
    //@{
    static void ReadFireClasses( xml::xistream& xis );
    void ReadExtinguisherAgent( xml::xistream& xis );
    void ReadWeatherEffect( xml::xistream& xis );
    void ReadInjury( xml::xistream& xis );
    void ReadUrbanModifier( xml::xistream& xis );
    void ReadSurface( xml::xistream& xis );
    //@}

private:
    T_ExtinguisherAgentEffectMap extinguisherAgentEffects_;
    T_WeatherEffectMap weatherEffects_;
    T_InjuryMap injuries_;
    T_UrbanModifierMap urbanModifiers_;
    T_SurfaceMap surfaces_;
    const std::string name_;
    int decreaseRate_;
    int increaseRate_;
    int initialHeat_;
    int maxHeat_;
    static T_FireClassMap classes_;
    static unsigned int cellSize_;
};

#endif // __MIL_FireClass_h_