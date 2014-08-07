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

#include <spatialcontainer/TerrainData.h>
#include <tools/Map.h>

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;
class PHY_HumanWound;
class TerrainData;
class PHY_UrbanAttritionData;
class PHY_MaterialCompositionType;

namespace weather
{
    class PHY_Precipitation;
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

    static int GetCellSize();
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
    const PHY_DotationCategory* FindBestExtinguisherAgent( std::function< bool( const PHY_DotationCategory& ) > isExtinguisherAgentOkFun ) const;
    int GetExtinguisherHeatDecreaseRate( const PHY_DotationCategory& extinguisherAgent ) const;
    const PHY_UrbanAttritionData& GetUrbanAttritionData() const;
    //@}

    //! @name Operations
    //@{
    const PHY_HumanWound* ChooseRandomWound() const;
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
    typedef std::map< std::string, const MIL_FireClass* > T_FireClassMap;

    struct ExtinguisherAgentEffect
    {
        const PHY_DotationCategory* pExtinguisherAgent_;
        int heatDecreaseRate_;
    };
    typedef std::vector< ExtinguisherAgentEffect > T_ExtinguisherAgentEffectVector;

    struct WeatherEffect
    {
        int heatDecreaseRate_;
    };
    typedef std::map< const weather::PHY_Precipitation*, WeatherEffect > T_WeatherEffectMap;

    struct Injury
    {
        double percentage_;
    };
    typedef tools::Map< const PHY_HumanWound*, Injury > T_InjuryMap;

    struct Surface
    {
        TerrainData terrainData_;
        int ignitionThreshold_;
        int maxCombustionEnergy_;
    };
    typedef std::vector<Surface > T_Surfaces;

private:
    //! @name Helpers
    //@{
    static void ReadFireClasses( xml::xistream& xis );
    void ReadExtinguisherAgent( xml::xistream& xis );
    void ReadWeatherEffect( xml::xistream& xis );
    void ReadInjury( xml::xistream& xis );
    void ReadUrbanModifiers( xml::xistream& xis );
    void ReadSurface( xml::xistream& xis );
    //@}

private:
    T_ExtinguisherAgentEffectVector extinguisherAgentEffects_;
    T_WeatherEffectMap weatherEffects_;
    T_InjuryMap injuries_;
    std::unique_ptr< PHY_UrbanAttritionData > urbanModifiers_;
    T_Surfaces surfaces_;
    const std::string name_;
    int decreaseRate_;
    int increaseRate_;
    int initialHeat_;
    int maxHeat_;
    static T_FireClassMap classes_;
    static unsigned int cellSize_;
};

#endif // __MIL_FireClass_h_
