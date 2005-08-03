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
             Position( const std::string& strMgrs );
             Position( double rLatitude, double rLongitude );
             Position( const geometry::Point2< double >& pt );
    virtual ~Position();
    //@}

    //! @name Static Operations
    //@{
    static void    Initialize( const std::string& strWorldConfigFile );
    static void    Terminate ();

    static geometry::Point2< double > Point2FromPolar( double rRadius, double rDegAngle );
    //@}

    //! @name Accessors
    //@{
    const  std::string& GetMgrsCoordinate () const;
           double       GetLatitude       () const;
           double       GetLongitude      () const;
    //@}

    //! @name Operations
    //@{
    double GetDistanceTo( const Position& position ) const;
    bool   IsOnSegment  ( const Position& start, const Position& end ) const;
    //@}

    //! @name Operators
    //@{
    Position& operator=( const Position& position );
    Position& operator=( const std::string& strMgrs );
    Position& operator=( const geometry::Point2< double >& pt );
    Position  operator+( const geometry::Point2< double >& pt ) const;
    //@}

private:
    //! @name Operations
    //@{
    static geometry::Point2< double > Point2FromMgrs  ( const std::string& strMgrs );
    static std::string                MgrsFromPoint2  ( const geometry::Point2< double >& pt );
    static double                     RadianFromDegree( double rDegree );
    static double                     DegreeFromRadian( double rRadian );

           void                       UpdateWGS84     ();
    //@}

private:
	//! @name Member data
    //@{
    std::string strMgrs_;
    double      rLatitude_;
    double      rLongitude_;

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