// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_Meteo_h_
#define __PHY_Meteo_h_

#include "clients_kernel/Entity_ABC.h"
#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Vector2D.h"
#include <geometry/Types.h>
#include <boost/noncopyable.hpp>

namespace Common
{
    class MsgWeatherAttributes;
    class MsgMissionParameters;
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
    class MeteoManager_ABC;
    class PHY_Lighting;
    class PHY_Precipitation;
    class PHY_RawVisionData_ABC;

// =============================================================================
/** @class  PHY_Meteo
    @brief  PHY Meteo
*/
// Created: JVT 03-08-05
// =============================================================================
class PHY_Meteo : public kernel::Entity_ABC
                , private boost::noncopyable
{
public:
    struct sWindData
    {
        MT_Float    rWindSpeed_;
        MT_Vector2D vWindDirection_;
    };

public:
    //! @name Constructors/Destructor
    //@{
             PHY_Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting& light, int conversionFactor );
             PHY_Meteo( unsigned int id, const Common::MsgWeatherAttributes&, MeteoManager_ABC* listener );
             PHY_Meteo( unsigned int id, const Common::MsgMissionParameters&, MeteoManager_ABC* listener );
             PHY_Meteo( const PHY_Lighting& light, PHY_Precipitation& precipitation );
    virtual ~PHY_Meteo();
    //@}

    //! @name Operations
    //@{
    void IncRef();
    void IncRef( unsigned int );
    void DecRef();
    void DecRef( unsigned int );
    void SetListener( MeteoManager_ABC* );
    //@}

    //! @name Accessors
    //@{
    const PHY_Precipitation& GetPrecipitation() const;
    const PHY_Lighting&      GetLighting     () const;
    const sWindData&         GetWind         () const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgWeatherAttributes& asn );
    virtual void Update( const Common::MsgMissionParameters& asn );
    virtual void Update( const PHY_Lighting& /*PHY_Ephemeride&*/ );
    virtual void Update( const PHY_Precipitation& precipitation);
    virtual void UpdateMeteoPatch( int date, PHY_RawVisionData_ABC& dataVision );
    virtual void SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    //@}

protected:
    //! @name Member data
    //@{
    int                      nPlancherCouvertureNuageuse_;
    int                      nPlafondCouvertureNuageuse_;
    MT_Float                 rDensiteCouvertureNuageuse_;
    sWindData                wind_;
    const PHY_Lighting*      pLighting_;
    const PHY_Precipitation* pPrecipitation_;
    unsigned int nRefCount_;
    int conversionFactor_;
    unsigned int id_;
    bool isChanged_;
    //@}

private:
    //! @name Member data
    //@{
    MeteoManager_ABC* listener_;
    //@}
};

#include "PHY_Meteo.inl"

}

#endif // __PHY_Meteo_h_