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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_Path_Channeler.h $
// $Author: Age $
// $Modtime: 24/06/05 11:42 $
// $Revision: 5 $
// $Workfile: DEC_Population_Path_Channeler.h $
//
// *****************************************************************************

#ifndef __DEC_Population_Path_Channeler_h_
#define __DEC_Population_Path_Channeler_h_

class TER_Localisation;

// =============================================================================
// Created: SBO 2005-01-16
// =============================================================================
class DEC_Population_Path_Channeler
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Population_Path_Channeler( const TER_Localisation& location );
    virtual ~DEC_Population_Path_Channeler();
    //@}

    //! @name Operations
    //@{
    MT_Float ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    TER_Localisation location_;
};

#endif // __DEC_Population_Path_Channeler_h_
