// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_PathfinderRule.h $
// $Author: Age $
// $Modtime: 24/06/05 11:42 $
// $Revision: 5 $
// $Workfile: DEC_Population_PathfinderRule.h $
//
// *****************************************************************************

#ifndef __DEC_Population_PathfinderRule_h_
#define __DEC_Population_PathfinderRule_h_

#include "pathfind/TerrainRule_ABC.h"
#include "DEC_Population_Path_Channeler.h"

// =============================================================================
// Created: AGE 2005-03-08
// =============================================================================
class DEC_Population_PathfinderRule : public TerrainRule_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Population_PathfinderRule();
    virtual ~DEC_Population_PathfinderRule();
    //@}

    //! @name Cost Configuration
    //@{
    void AddChanneler( const DEC_Population_Path_Channeler& channeler );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_Population_PathfinderRule( const DEC_Population_PathfinderRule& );            //!< Copy constructor
    DEC_Population_PathfinderRule& operator=( const DEC_Population_PathfinderRule& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to );
    virtual float GetCost     ( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween );
    //@}


private:
    //! @name Types
    //@{
    typedef std::vector< DEC_Population_Path_Channeler >  T_PathChannelers;
    typedef T_PathChannelers::const_iterator              CIT_PathChannelers;
    //@}

private:
    T_PathChannelers channelers_;
};

#endif // __DEC_Population_PathfinderRule_h_
