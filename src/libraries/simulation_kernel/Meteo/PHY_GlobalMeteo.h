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

class MeteoManager_ABC;
class PHY_Meteo;
class PHY_Lighting;
class PHY_RawVisionData_ABC;

// =============================================================================
/** @class  PHY_GlobalMeteo
    @brief  PHY_GlobalMeteo
*/
// Created: HBD 2010-03-25
// =============================================================================
class PHY_GlobalMeteo : public PHY_Meteo
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_GlobalMeteo(  xml::xistream& xis, const PHY_Lighting& light, int conversionFactor );
             explicit PHY_GlobalMeteo( const Common::MsgMeteoAttributes&, MeteoManager_ABC* list );
    virtual ~PHY_GlobalMeteo();
    //@}
    //! @name Operations
    //@{
    void SendRegisterGlobal();
    void SendUnregisterGlobal();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_GlobalMeteo( const PHY_GlobalMeteo& );            //!< Copy constructor
    PHY_GlobalMeteo& operator=( const PHY_GlobalMeteo& ); //!< Assignment operator
    //@}
};

#endif // __PHY_GlobalMeteo_h_
