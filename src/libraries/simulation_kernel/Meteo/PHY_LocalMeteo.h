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

namespace Common
{
    class MsgMeteoAttributes;
}

namespace xml
{
    class xistream;
}

class MeteoManager_ABC;
class PHY_Meteo;
class PHY_Lighting;
class PHY_RawVisionData_ABC;

// =============================================================================
/** @class  PHY_LocalMeteo
    @brief  PHY_LocalMeteo
*/
// Created: SLG 2010-03-18
// =============================================================================
class PHY_LocalMeteo : public PHY_Meteo
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_LocalMeteo( xml::xistream& xis, const PHY_Lighting& light, int conversionFactor );
    explicit PHY_LocalMeteo( const Common::MsgMeteoAttributes&, MT_Vector2D upLeft, MT_Vector2D upRight  );
    virtual ~PHY_LocalMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateMeteoPatch( int date, PHY_RawVisionData_ABC& dataVision_ );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_LocalMeteo( const PHY_LocalMeteo& );            //!< Copy constructor
    PHY_LocalMeteo& operator=( const PHY_LocalMeteo& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //PHY_Meteo* meteo_;
    int startTime_;
    int endTime_;
    MT_Vector2D upLeft_;
    MT_Vector2D downRight_;
    bool bIsPatched_;
    MeteoManager_ABC* listener_;
    //@}
};

#endif // __PHY_LocalMeteo_h_