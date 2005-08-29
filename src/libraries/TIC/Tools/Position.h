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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Position.h $
// $Author: Sbo $
// $Modtime: 18/07/05 17:42 $
// $Revision: 14 $
// $Workfile: Position.h $
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "Types.h"
#include "geometry/Point2.h"
#include "geocoord/PlanarCartesian.h"

#define PI 3.141592654

namespace geocoord
{
    class MGRS;
}

namespace TIC
{
// =============================================================================
/** @class  Position
    @brief  Position
    @par    Using example
    @code
    Position;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class Position : public geometry::Point2< double >
{
public:
	//! @name Constructors/Destructor
    //@{
             Position();
             Position( const Position& position );
             Position( const std::string& strMgrs );
             Position( double rLatitude, double rLongitude );
    virtual ~Position();
    //@}

    //! @name Static Operations
    //@{
    static void     Initialize       ( const std::string& strWorldConfigFile );
    static void     Terminate        ();
    static Position PositionFromPolar( double rRadius, double rDegAngle );
    //@}

    //! @name Accessors
    //@{
    const std::string GetMgrsCoordinate() const;
          double      GetLatitude      () const;
          double      GetLongitude     () const;
          double      GetSimX          () const;
          double      GetSimY          () const;
    //@}

    //! @name Modifiers
    //@{
    void SetSimCoordinates( double rX, double rY );
    //@}

    //! @name Operations
    //@{
    double GetDistanceTo( const Position& position ) const;
    bool   IsOnSegment  ( const Position& start, const Position& end ) const;
    //@}

    //! @name Operators
    //@{
    Position& operator=( const Position& position );
    Position  operator+( const Position& position ) const;
    //@}

private:
    //! @name Static Operations
    //@{
    static double RadianFromDegree( double rDegree );
    static double DegreeFromRadian( double rRadian );
    //@}

   //! @name Coordinate Conversion Operations
   //@{
   Position&   PositionFromMgrs( const std::string& strMgrs  );
   std::string MgrsFromPosition( const Position&    position ) const;
   Position&   PositionFromWGS ( double rLatitude, double rLongitude );
   //@}

private:
	//! @name Member data
    //@{
    // reference values
    static geocoord::PlanarCartesian*             pPlanar_;
    static geocoord::PlanarCartesian::Parameters* pParameters_;
    static geometry::Point2< double >*            pTranslation_;
    static geocoord::Geodetic*                    pGeodetic_;
	//@}
};


} // end namespace TIC

#include "Position.inl"

#endif // __Position_h_