// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_Analyzer_h_
#define __TER_Analyzer_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/noncopyable.hpp>

class TerrainAnalyzer;
class TerrainData;
class TER_Localisation;
class TER_Polygon;
class TER_StaticData;

// =============================================================================
/** @class  TER_Analyzer
    @brief  %TER_Analyzer
*/
// Created: CMA 2011-08-16
// =============================================================================
class TER_Analyzer : boost::noncopyable
{
public:
    typedef std::function< void( const MT_Vector2D& pos, const TerrainData& data ) > T_Functor;

    static TER_Analyzer& GetAnalyzer();

    //! @name Constructors/Destructor
    //@{
    explicit TER_Analyzer( const TER_StaticData& staticData );
    virtual ~TER_Analyzer();
    //@}

    //! @name Operations
    //@{
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, const T_Functor& bestNodeFunction ) const;

    std::vector< boost::shared_ptr< MT_Vector2D > > FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius );
    void FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance, std::vector< boost::shared_ptr< MT_Vector2D > >& positions );
    void FindRoadsOnBorderOfPolygon( const TER_Polygon& polygon, std::vector< boost::shared_ptr< MT_Vector2D > >& positions );
    void FindSegmentIntersections( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& terrainSought, std::vector< boost::shared_ptr< MT_Vector2D > >& positions );
    std::vector< boost::shared_ptr< MT_Vector2D > > FindAllPositions( const MT_Vector2D& center, float radius );

    TerrainData FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius );
    TerrainData FindTerrainDataWithinPolygon( const TER_Polygon& polygon );
    TerrainData Pick( const MT_Vector2D& pos );
    TerrainData GetTerrainData( const TER_Localisation& localisation );
    //@}

private:
    //! @name Member data
    //@{
    TerrainAnalyzer* pAnalyzer_;
    //@}
};

#endif // __TER_Analyzer_h_
