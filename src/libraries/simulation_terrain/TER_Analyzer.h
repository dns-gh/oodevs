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

class TerrainAnalyzer;
class TerrainRetractationHandle;
class TER_NodeFunctor_ABC;
class TER_StaticData;
class TerrainData;

// =============================================================================
/** @class  TER_Analyzer
    @brief  %TER_Analyzer
*/
// Created: CMA 2011-08-16
// =============================================================================
class TER_Analyzer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TER_Analyzer( const TER_StaticData& staticData );
    virtual ~TER_Analyzer();
    //@}

    //! @name Operations
    //@{
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const;

    std::vector< boost::shared_ptr< MT_Vector2D > > FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius );
    std::vector< boost::shared_ptr< MT_Vector2D > > FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance );
    std::vector< boost::shared_ptr< MT_Vector2D > > FindAllPositions( const MT_Vector2D& center, float radius );
    TerrainData FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_Analyzer( const TER_Analyzer& );            //!< Copy constructor
    TER_Analyzer& operator=( const TER_Analyzer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TerrainAnalyzer* pAnalyzer_;
    //@}
};

#endif // __TER_Analyzer_h_
