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

#pragma warning( disable : 4996 )

#include "clients_kernel/Entity_ABC.h"
#include "MT_Tools/MT_Vector2D.h"
#include <geometry/Types.h>
#include <cassert>
#include <boost/shared_ptr.hpp>

class QString;

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
}

namespace weather
{
    class PHY_Lighting;
    class PHY_Precipitation;
    class PHY_RawVisionData_ABC;

// =============================================================================
/** @class  Meteo
    @brief  Meteo
*/
// Created: ABR 2011-06-01
// =============================================================================
class Meteo : public kernel::Entity_ABC
{
public:
    struct sWindData
    {
        double       rSpeed_;
        unsigned int eAngle_;
        MT_Vector2D  vDirection_;
        sWindData() : rSpeed_( 0.), eAngle_( 0 ) {}
    };
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
             Meteo( unsigned int id, unsigned int timeStep, const std::string& name = "" );
             Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting* light, unsigned int timeStep, const std::string& name = "" );
             Meteo( unsigned int id, const PHY_Lighting& light, const PHY_Precipitation& precipitation, unsigned int timeStep, const std::string& name = "" );
             Meteo( unsigned int id, const sword::WeatherAttributes&, unsigned int timeStep, const std::string& name = "" );
             Meteo( unsigned int id, const sword::MissionParameters&, const PHY_Lighting& light, unsigned int timeStep, const std::string& name = ""  );
    virtual ~Meteo();
    //@}

    //! @name Accessors
    //@{
    const PHY_Precipitation& GetPrecipitation   () const;
    const PHY_Lighting&      GetLighting        () const;
    const sWindData&         GetWind            () const;
    const sCloudData&        GetCloud           () const;
    double                   GetConversionFactor() const;
    bool                     IsModified() const;

    void SetPrecipitation( const PHY_Precipitation& precipitation );
    void SetLighting     ( const PHY_Lighting& light );
    void SetWind         ( const sWindData& wind );
    void SetCloud        ( const sCloudData& cloud );
    void SetModified     ( bool modified );
    //@}

    //! @name Entity implementation
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}

    //! @name Operations
    //@{
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Update( const sword::WeatherAttributes& msg );
    virtual void Update( const sword::MissionParameters& asn );
    virtual void Update( const PHY_Lighting& lighting );
    virtual void Update( const PHY_Precipitation& precipitation);
    virtual void UpdateMeteoPatch( int date, PHY_RawVisionData_ABC& dataVision, boost::shared_ptr< Meteo > meteo );
    virtual void SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const;
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
    sWindData                wind_;
    sCloudData               cloud_;
    const PHY_Lighting*      pLighting_;
    const PHY_Precipitation* pPrecipitation_;
    double                   conversionFactor_;
    bool                     modified_;
    //@}
};

#include "Meteo.inl"

}
#endif // __weather_Meteo_h_
