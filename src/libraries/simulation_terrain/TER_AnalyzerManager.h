// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TER_AnalyzerManager_h_
#define __TER_AnalyzerManager_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include "TER_Analyzer.h"

class TerrainData;
class TER_StaticData;

// =============================================================================
/** @class  TER_AnalyzerManager
    @brief  %TER_AnalyzerManager
*/
// Created: CMA 2011-08-16
// =============================================================================
class TER_AnalyzerManager
{
public:
    //! @name Helpers
    //@{
    static TerrainData& DefaultTerrainData();
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit TER_AnalyzerManager( const TER_StaticData& staticData );
    virtual ~TER_AnalyzerManager();
    //@}

    //! @name Operations
    //@{
    static TER_AnalyzerManager& GetAnalyzerManager();

    template< typename Functor >
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, Functor& bestNodeFunction ) const;

    std::vector< boost::shared_ptr< MT_Vector2D > > FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius );
    void FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance, std::vector< boost::shared_ptr< MT_Vector2D > >& positions );
    void FindRoadsOnBorderOfPolygon( const TER_Polygon& polygon, std::vector< boost::shared_ptr< MT_Vector2D > >& positions );
    void FindRiverSegmentIntersections( const MT_Vector2D& from, const MT_Vector2D& to, std::vector< boost::shared_ptr< MT_Vector2D > >& positions );
    std::vector< boost::shared_ptr< MT_Vector2D > > FindAllPositions( const MT_Vector2D& center, float radius );

    TerrainData FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius );
    TerrainData FindTerrainDataWithinPolygon( const TER_Polygon& polygon );
    TerrainData Pick( const MT_Vector2D& pos );
    TerrainData GetTerrainData( const TER_Localisation& localisation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_AnalyzerManager( const TER_AnalyzerManager& );            //!< Copy constructor
    TER_AnalyzerManager& operator=( const TER_AnalyzerManager& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const TER_StaticData& staticData_;
    TER_Analyzer* pAnalyzer_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::ApplyOnNodesWithinCircle
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
template < typename Functor >
void TER_AnalyzerManager::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, Functor& bestNodeFunction ) const
{
    TER_NodeFunctor< Functor > functor( bestNodeFunction );
    pAnalyzer_->ApplyOnNodesWithinCircle( vCenter, rRadius, functor );
}

#endif // __TER_AnalyzerManager_h_
