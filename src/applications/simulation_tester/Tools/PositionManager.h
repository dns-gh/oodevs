// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/PositionManager.h $
// $Author: Sbo $
// $Modtime: 18/07/05 10:14 $
// $Revision: 6 $
// $Workfile: PositionManager.h $
//
// *****************************************************************************

#ifndef __PositionManager_h_
#define __PositionManager_h_

#include "Types.h"
#include "geometry/Point2.h"
#include "geocoord/PlanarCartesian.h"

#define PI 3.141592654

namespace geocoord
{
    class MGRS;
}

namespace TEST
{
    class Position;

// =============================================================================
/** @class  PositionManager
    @brief  PositionManager
    @par    Using example
    @code
    PositionManager;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class PositionManager
{
    MT_COPYNOTALLOWED( PositionManager );

public:
	//! @name Constructors/Destructor
    //@{
             PositionManager( const std::string& strWorldConfigFile );
    virtual ~PositionManager();
    //@}

    //! @name Accessors
    //@{
    double GetWorldWidth () const;
    double GetWorldHeight() const;
    //@}

    //! @name Coordinate Conversion
    //@{
    Position&    PositionFromMgrs( const std::string& strMgrs  );
    std::string  MgrsFromPosition( const Position&    position );
    Position&    PositionFromWGS ( double rLatitude, double rLongitude );
    /* // TODO: add if needed
    std::string  MgrsFromWGS     ( double rLatitude, double rLongitude );
    */

    double       RadianFromDegree( double rDegree );
    double       DegreeFromRadian( double rRadian );
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeCoordinateConverter( const std::string& strWorldConfigFile );
    //@}

private:
    //! @name Member data
    //@{
    double rWorldWidth_;
    double rWorldHeight_;
    //@}

    //! @name reference values
    //@{
    geocoord::PlanarCartesian*             pPlanar_;
    geocoord::PlanarCartesian::Parameters* pParameters_;
    geometry::Point2< double >*            pTranslation_;
    geocoord::Geodetic*                    pGeodetic_;
    //@}

};

} // end namespace TIC

#include "PositionManager.inl"

#endif // __PositionManager_h_