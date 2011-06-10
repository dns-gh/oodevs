// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_LocalMeteo_h_
#define __PHY_LocalMeteo_h_

#include "meteo/Meteo.h"

namespace sword
{
    class MissionParameters;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  PHY_LocalMeteo
    @brief  PHY_LocalMeteo
*/
// Created: SLG 2010-03-18
// =============================================================================
class PHY_LocalMeteo : public weather::Meteo
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_LocalMeteo( unsigned int id, xml::xistream& xis, const weather::PHY_Lighting& light, unsigned int timeStep );
             PHY_LocalMeteo( unsigned int id, const sword::MissionParameters& message, const weather::PHY_Lighting& light, unsigned int timeStep );
    virtual ~PHY_LocalMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateMeteoPatch( int date, weather::PHY_RawVisionData_ABC& dataVision, boost::shared_ptr< weather::Meteo > meteo );
    virtual void Update( const sword::MissionParameters& msg );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SendCreation() const;
    virtual void SendDestruction();
    void LocalUpdate( const sword::MissionParameters& msg );
    //@}

private:
    //! @name Member data
    //@{
    int startTime_;
    int endTime_;
    MT_Vector2D upLeft_;
    MT_Vector2D downRight_;
    bool bIsPatched_;
    //@}
};

#endif // __PHY_LocalMeteo_h_