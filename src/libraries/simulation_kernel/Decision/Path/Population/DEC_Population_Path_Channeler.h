// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Population_Path_Channeler_h_
#define __DEC_Population_Path_Channeler_h_

#include "MIL.h"

class TER_Localisation;
class DEC_Population_PathClass;

// =============================================================================
// Created: SBO 2005-01-16
// =============================================================================
class DEC_Population_Path_Channeler
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Population_Path_Channeler( const DEC_Population_PathClass& pathClass, const TER_Localisation& location );
    virtual ~DEC_Population_Path_Channeler();
    //@}

    //! @name Operations
    //@{
    double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    TER_Localisation location_;
    double rCostOutsideChanneler_;
};

#endif // __DEC_Population_Path_Channeler_h_
