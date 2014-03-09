// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/MIL_PopulationType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:14 $
// $Revision: 3 $
// $Workfile: MIL_PopulationType.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationType_h_
#define __MIL_PopulationType_h_

#include "MIL_PopulationPionAttritionData.h"

namespace xml
{
    class xistream;
}

class DEC_Model_ABC;
class MIL_Army_ABC;
class MIL_Population;
class MIL_PopulationAttitude;
class PHY_Volume;
class PHY_Protection;
class PHY_RoePopulation;
class PHY_Speeds;
class TerrainData;

// =============================================================================
// @class  MIL_PopulationType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PopulationType : private boost::noncopyable
{
public:
    //! @name Accessors
    //@{
    virtual ~MIL_PopulationType();
    //@}

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();
    static const MIL_PopulationType* Find( const std::string& strName );
    static const MIL_PopulationType* Find( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    double GetArmedIndividuals() const;
    double GetMale() const;
    double GetFemale() const;
    double GetChildren() const;
    double GetConcentrationDensity() const;
    double GetDefaultFlowDensity() const;
    double GetMaxSpeed() const;
    double GetMaxSpeed( const TerrainData& data ) const;
    double GetDelay() const;
    bool CanCollideWithFlow() const;
    const DEC_Model_ABC& GetModel() const;
    //@}

    //! @name Effects
    //@{
    double GetPionMaxSpeed( const MIL_PopulationAttitude& populationAttitude, double rPopulationDensity, const PHY_Volume& pionVolume ) const;
    double GetPionReloadingTimeFactor( double rPopulationDensity ) const;
    const PHY_AttritionData GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection, double armedIndividuals ) const;
    double GetPH( const MIL_PopulationAttitude& attitude, double rDensity ) const;
    double GetDamageSurface( const PHY_RoePopulation& roeFirer ) const;
    double GetDamagePH( const PHY_RoePopulation& roeFirer ) const;
    double GetUrbanDestructionDensity( const MIL_PopulationAttitude& attitude ) const;
    double GetUrbanDestructionTime( const MIL_PopulationAttitude& attitude ) const;
    double GetDecontaminationDelay() const;
    //@}

protected:
    MIL_PopulationType( const DEC_Model_ABC& model, double rConcentrationDensity = 0. );

private:
     MIL_PopulationType( const std::string& strName, xml::xistream& xis );

    //! @name Initialization
    //@{
    void InitializeSlowDownData ( xml::xistream& xis );
    void InitializeFireData     ( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    struct sSlowDownData
    {
        sSlowDownData( double rPopulationDensity, double rMaxSpeed )
            : rPopulationDensity_( rPopulationDensity )
            , rMaxSpeed_( rMaxSpeed )
        {
            // NOTHING
        }
        double rPopulationDensity_;
        double rMaxSpeed_;
    };
    typedef std::vector< sSlowDownData >        T_VolumeSlowDownData;
    typedef std::vector< T_VolumeSlowDownData > T_AttitudeSlowDownData;
    struct sDamageData
    {
        sDamageData( double rAttritionSurface, double rPH )
            : rAttritionSurface_( rAttritionSurface )
            , rPH_( rPH )
        {
            // NOTHING
        }
        double rAttritionSurface_;
        double rPH_;
    };
    typedef std::vector< sDamageData > T_DamageData;
    struct sUrbanDestructionData
    {
        sUrbanDestructionData( double rDensity, double rTime )
            : rDensity_( rDensity )
            , rTime_( rTime )
        {
            // NOTHING
        }
        double rDensity_;
        double rTime_;
    };
    typedef std::vector< sUrbanDestructionData > T_UrbanDestructionData;
    //@}

    //! @name Helpers
    //@{
    static void ReadPopulation( xml::xistream& xis );
    void ReadSlowingEffect    ( xml::xistream& xis );
    void ReadSlowingUnitEffect( xml::xistream& xis, T_VolumeSlowDownData& volumeSlowDownData );
    void ReadUnitFireEffect   ( xml::xistream& xis );
    void ReadUrbanDestructionEffect ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    unsigned int nID_;
    double rConcentrationDensity_;
    double rDefaultFlowDensity_;
    std::unique_ptr< PHY_Speeds > speeds_;
    double rArmedIndividuals_;
    double rMale_;
    double rFemale_;
    double rChildren_;
    T_AttitudeSlowDownData slowDownData_;
    MIL_PopulationPionAttritionData attritionData_;
    T_DamageData damageData_;
    T_UrbanDestructionData urbanDestructionData_;
    const DEC_Model_ABC* pModel_;
    double decontaminationDelay_;
    bool canCollideWithFlow_;
    //@}
};

#endif // __MIL_PopulationType_h_
