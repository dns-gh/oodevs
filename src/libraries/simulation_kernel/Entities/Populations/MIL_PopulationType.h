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

class DEC_DataBase;
class DEC_Model_ABC;
class MIL_Army;
class MIL_Population;
class MIL_PopulationAttitude;
class PHY_Volume;
class PHY_Protection;
class PHY_RoePopulation;

// =============================================================================
// @class  MIL_PopulationType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PopulationType : private boost::noncopyable
{

public:
    //! @name Manager
    //@{
    static void                      Initialize( xml::xistream& xis );
    static void                      Terminate ();
    static const MIL_PopulationType* Find      ( const std::string& strName );
    static const MIL_PopulationType* Find      ( uint nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&                         GetName                () const;
          uint                                 GetID                  () const;
          MT_Float                             GetConcentrationDensity() const;
          MT_Float                             GetDefaultFlowDensity  () const;
          MT_Float                             GetMaxSpeed            () const;

    const DEC_Model_ABC&                       GetModel               () const;
    //@}

    //! @name Operations
    //@{
          MIL_Population& InstanciatePopulation( xml::xistream& xis, MIL_Army& army, DEC_DataBase& database ) const;
    //@}

    //! @name Effects
    //@{
          MT_Float           GetPionMaxSpeed           ( const MIL_PopulationAttitude& populationAttitude, MT_Float rPopulationDensity, const PHY_Volume& pionVolume ) const;
          MT_Float           GetPionReloadingTimeFactor( MT_Float rPopulationDensity ) const;

    const PHY_AttritionData& GetAttritionData          ( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection ) const;
          MT_Float           GetPH                     ( const MIL_PopulationAttitude& attitude, MT_Float rDensity ) const;

          MT_Float           GetDamageSurface          ( const PHY_RoePopulation& roeFirer ) const;
          MT_Float           GetDamagePH               ( const PHY_RoePopulation& roeFirer ) const;
    //@}

protected:
    MIL_PopulationType( const DEC_Model_ABC& model );
    ~MIL_PopulationType();

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_PopulationType*, sCaseInsensitiveLess > T_PopulationMap;
    typedef T_PopulationMap::const_iterator                                          CIT_PopulationMap;
    //@}

private:
     MIL_PopulationType( const std::string& strName, xml::xistream& xis );

    //! @name Initialization
    //@{
    void InitializeSlowDownData ( xml::xistream& xis );
    void InitializeFireData     ( xml::xistream& xis );
    void InitializeDiaFunctions ();
    //@}

private:
    //! @name Types
    //@{
    struct sSlowDownData
    {
        sSlowDownData( MT_Float rPopulationDensity, MT_Float rMaxSpeed ) : rPopulationDensity_( rPopulationDensity ), rMaxSpeed_( rMaxSpeed ) {}
        MT_Float rPopulationDensity_;
        MT_Float rMaxSpeed_;
    };
    typedef std::vector< sSlowDownData >        T_VolumeSlowDownData;
    typedef std::vector< T_VolumeSlowDownData > T_AttitudeSlowDownData;
    struct sDamageData
    {
        sDamageData( MT_Float rAttritionSurface, MT_Float rPH ) : rAttritionSurface_( rAttritionSurface ), rPH_( rPH ) {}
        MT_Float rAttritionSurface_;
        MT_Float rPH_;
    };
    typedef std::vector< sDamageData >          T_DamageData;
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadPopulation( xml::xistream& xis );
    void ReadSlowingEffect    ( xml::xistream& xis );
    void ReadSlowingUnitEffect( xml::xistream& xis, T_VolumeSlowDownData& volumeSlowDownData );
    void ReadUnitFireEffect   ( xml::xistream& xis );
    //@}

private:
    const std::string                          strName_;
          uint                                 nID_;
          MT_Float                             rConcentrationDensity_;
          MT_Float                             rDefaultFlowDensity_;
          MT_Float                             rMaxSpeed_;
          T_AttitudeSlowDownData               slowDownData_;
          MIL_PopulationPionAttritionData      attritionData_;
          T_DamageData                         damageData_;

    const DEC_Model_ABC*                       pModel_;

private:
    static T_PopulationMap populations_;
    static MT_Float        rEffectReloadingTimeDensity_;
    static MT_Float        rEffectReloadingTimeFactor_;    
};

#endif // __MIL_PopulationType_h_
