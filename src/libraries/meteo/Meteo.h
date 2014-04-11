// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __weather_Meteo_h_
#define __weather_Meteo_h_

#include "MT_Tools/MT_Vector2D.h"
#include <geometry/Types.h>
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>

namespace sword
{
    class WeatherAttributes;
    class MissionParameters;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace kernel
{
    class ActionController;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace weather
{
    class PHY_Lighting;
    class PHY_Precipitation;

struct WindData
{
    double       rSpeed_;
    unsigned int eAngle_;
    MT_Vector2D  vDirection_;
    WindData() : rSpeed_( 0.), eAngle_( 0 ) {}
};

// =============================================================================
/** @class  Meteo
    @brief  Meteo
*/
// Created: ABR 2011-06-01
// =============================================================================
class Meteo
{
public:
    struct sCloudData
    {
        int          nFloor_;
        int          nCeiling_;
        int          nDensityPercentage_;
        double       rDensity_;
        sCloudData() : nFloor_( 0 ), nCeiling_( 0 ), nDensityPercentage_( 0 ), rDensity_( 0. ) {}
    };

public:
    //! @name Constructors/Destructor
    //@{
             Meteo();
             Meteo( unsigned int id, unsigned int timeStep, const std::string& name = "" );
             Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting* light, unsigned int timeStep,
                    const std::string& name = "" );
             Meteo( unsigned int id, const PHY_Lighting& light, const PHY_Precipitation& precipitation,
                    unsigned int timeStep, unsigned int temperature, const std::string& name = "" );
             Meteo( unsigned int id, const sword::WeatherAttributes&, unsigned int timeStep, const std::string& name = "" );
             Meteo( unsigned int id, const sword::MissionParameters&, const PHY_Lighting& light, unsigned int timeStep,
                    const std::string& name = "" );
    virtual ~Meteo();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive > void load( Archive&, const unsigned int );
    template< typename Archive > void save( Archive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;
    const PHY_Precipitation& GetPrecipitation   () const;
    const PHY_Lighting&      GetLighting        () const;
    const WindData&          GetWind            () const;
    int                      GetTemperature     () const;
    const sCloudData&        GetCloud           () const;
    double                   GetConversionFactor() const;
    bool                     IsModified() const;

    void SetPrecipitation( const PHY_Precipitation& precipitation );
    void SetLighting     ( const PHY_Lighting& light );
    void SetWind         ( const WindData& wind );
    void SetTemperature  ( int temperature );
    void SetCloud        ( const sCloudData& cloud );
    void SetModified     ( bool modified );
    //@}

    //! @name Operations
    //@{
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Update( const sword::WeatherAttributes& msg );
    virtual void Update( const sword::MissionParameters& asn );
    virtual void Update( const Meteo& );
            void SendCreationIfModified();
    virtual void SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction() const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Meteo& operator=( const Meteo& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    unsigned int             id_;
    std::string              name_;
    WindData                 wind_;
    sCloudData               cloud_;
    int                      temperature_;
    const PHY_Lighting*      pLighting_;
    const PHY_Precipitation* pPrecipitation_;
    double                   conversionFactor_;
    bool                     modified_;
    //@}
};

}

// -----------------------------------------------------------------------------
// Name: Meteo::serialize
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
template< typename Archive >
void weather::Meteo::load( Archive& file, const unsigned int )
{
    unsigned int lighting = 0;
    unsigned int precipitation = 0;
    file >> id_
         >> name_
         >> wind_.eAngle_
         >> wind_.rSpeed_
         >> wind_.vDirection_
         >> cloud_.nCeiling_
         >> cloud_.nDensityPercentage_
         >> cloud_.nFloor_
         >> cloud_.rDensity_
         >> temperature_
         >> lighting
         >> precipitation
         >> conversionFactor_
         >> modified_;
    pLighting_ = PHY_Lighting::FindLighting( protocol::FromProto(
                static_cast< sword::WeatherAttributes::EnumLightingType >( lighting) ));
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( protocol::FromProto(
                static_cast< sword::WeatherAttributes::EnumPrecipitationType >( precipitation ) ));
}

// -----------------------------------------------------------------------------
// Name: Meteo::serialize
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
template< typename Archive >
void weather::Meteo::save( Archive& file, const unsigned int ) const
{
    unsigned int lighting = protocol::ToProto( pLighting_->GetID() );
    unsigned int precipitation = protocol::ToProto( pPrecipitation_->GetID() );
    file <<  id_
         << name_
         << wind_.eAngle_
         << wind_.rSpeed_
         << wind_.vDirection_
         << cloud_.nCeiling_
         << cloud_.nDensityPercentage_
         << cloud_.nFloor_
         << cloud_.rDensity_
         << temperature_
         << lighting
         << precipitation
         << conversionFactor_
         << modified_;
}

BOOST_CLASS_EXPORT_KEY( weather::Meteo )

#endif // __weather_Meteo_h_
