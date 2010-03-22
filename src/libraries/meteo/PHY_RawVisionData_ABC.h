// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RawVisionData_ABC_h_
#define __PHY_RawVisionData_ABC_h_

#include "geometry/Types.h"

class PHY_Meteo;
// =============================================================================
/** @class  PHY_RawVisionData_ABC
    @brief  PHY_RawVisionData_ABC
*/
// Created: SLG 2010-03-18
// =============================================================================
class PHY_RawVisionData_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData_ABC(){}
    virtual ~PHY_RawVisionData_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterMeteoPatch   ( const geometry::Point2d&, const geometry::Point2d&, PHY_Meteo* ) = 0;
    virtual void UnregisterMeteoPatch( const geometry::Point2d&, const geometry::Point2d&, PHY_Meteo* ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RawVisionData_ABC( const PHY_RawVisionData_ABC& );            //!< Copy constructor
    PHY_RawVisionData_ABC& operator=( const PHY_RawVisionData_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __PHY_RawVisionData_ABC_h_
