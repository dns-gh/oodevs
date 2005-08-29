// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceSubCategory.h $
// $Author: Sbo $
// $Modtime: 4/07/05 10:19 $
// $Revision: 1 $
// $Workfile: DeviceSubCategory.h $
//
// *****************************************************************************

#ifndef __DeviceSubCategory_h_
#define __DeviceSubCategory_h_

namespace TIC
{

// =============================================================================
// @class  DeviceSubCategory
// Created: SBO 2005-06-30
// =============================================================================
class DeviceSubCategory
{
    MT_COPYNOTALLOWED( DeviceSubCategory )

public:
    //! @name constant
    //@{
    static const DeviceSubCategory  notDefined;
    //@}

public:
    //! @name Manager
    //@{
    static       void            Initialize();
    static       void            Terminate ();

    static const DeviceSubCategory* Find      ( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&   GetName   () const;
    const std::string&   GetKey    () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNone,
        eAirborneEarlyWarningAndControl,
        eAirTransportMediumRange,
        eAirTransportLongRange,
        eAirTransportShortRange,
        eAntiArmour,
        eAntiSubmarine,
        eAttackAircraft,
        eAttackHelicopter,
        eAutogyro,
        eBomberMediumRange,
        eBomberLongRange,
        eBomberShortRange,
        eCommandAndControl,
        eDrone,
        eElectronicCounterMeasures,
        eElectronicWarfare,
        eFighterBomber,
        eFighter,
        eHelicopterHeavyLift,
        eHelicopterLightLift,
        eHelicopterMediumLift,
        eHelicopterReconnaissance,
        eImageryIntelligenceGathering,
        eMedicalEvacuation,
        eNotKnown,
        eNotOtherwiseSpecified,
        eReconnaissance,
        eSearchAndRescue,
        eSignalsIntelligenceGathering,
        eTanker,
        eTrainer,
        eUnmannedAerialVehicle,
        eUtility
    };

    typedef std::map< std::string, const DeviceSubCategory*, sCaseInsensitiveLess > T_DeviceSubCategoryMap;
    typedef T_DeviceSubCategoryMap::const_iterator                                  CIT_DeviceSubCategoryMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     DeviceSubCategory( const std::string& strName, E_Type nType, const std::string& strKey );
    ~DeviceSubCategory();
    //@}

private:
    //! @name Member data
    //@{
    const std::string   strName_;
    const E_Type        nType_;
    const std::string   strKey_;

    static T_DeviceSubCategoryMap  deviceSubCategories_;
    //@}
};

} // end namespace TIC

#include "DeviceSubCategory.inl"

#endif // __DeviceSubCategory_h_
