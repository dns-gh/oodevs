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

#include "meteo/PHY_Meteo.h"

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
class PHY_LocalMeteo : public weather::PHY_Meteo
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_LocalMeteo( unsigned int id, xml::xistream& xis, const weather::PHY_Lighting& light );
             PHY_LocalMeteo( unsigned int id, const sword::MissionParameters& message, weather::MeteoManager_ABC* list );
    virtual ~PHY_LocalMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateMeteoPatch( int date, weather::PHY_RawVisionData_ABC& dataVision );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_LocalMeteo( const PHY_LocalMeteo& );            //!< Copy constructor
    PHY_LocalMeteo& operator=( const PHY_LocalMeteo& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SendCreation() const;
    virtual void SendDestruction();
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