// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_GlobalMeteo_h_
#define __PHY_GlobalMeteo_h_

#include "meteo/PHY_Meteo.h"

namespace xml
{
    class xistream;
}

namespace weather
{
    class MeteoManager_ABC;
    class PHY_Meteo;
    class PHY_Lighting;
    class PHY_RawVisionData_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  PHY_GlobalMeteo
    @brief  PHY_GlobalMeteo
*/
// Created: HBD 2010-03-25
// =============================================================================
class PHY_GlobalMeteo : public weather::PHY_Meteo
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_GlobalMeteo( unsigned int id, xml::xistream& xis, const weather::PHY_Lighting& light, int conversionFactor );
             PHY_GlobalMeteo( unsigned int id, const Common::MsgMeteoAttributes&, weather::MeteoManager_ABC* list );
    virtual ~PHY_GlobalMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgMissionParameters& asn );
    virtual void UpdateMeteoPatch( int date, weather::PHY_RawVisionData_ABC& dataVision );
    virtual void SendCreation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_GlobalMeteo( const PHY_GlobalMeteo& );            //!< Copy constructor
    PHY_GlobalMeteo& operator=( const PHY_GlobalMeteo& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool _isChanged;
    //@}
};

#endif // __PHY_GlobalMeteo_h_
