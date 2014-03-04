// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationManager.h $
// $Author: Nld $
// $Modtime: 11/04/05 15:27 $
// $Revision: 5 $
// $Workfile: TER_PopulationManager.h $
//
// *****************************************************************************

#ifndef __TER_PopulationManager_h_
#define __TER_PopulationManager_h_

#include "TER_PopulationConcentrationManager.h"
#include "TER_PopulationFlowManager.h"

class MT_Rect;

// =============================================================================
// Created: NLD 2005-10-07
// =============================================================================
class TER_PopulationManager
{
public:
    //! @name Constructor/destructor
    //@{
     TER_PopulationManager( const MT_Rect& extent );
    virtual ~TER_PopulationManager();
    //@}

    //! @name Accessors
    //@{
    TER_PopulationConcentrationManager& GetConcentrationManager();
    TER_PopulationFlowManager&          GetFlowManager         ();
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    TER_PopulationManager( const TER_PopulationManager& );            //!< Copy constructor
    TER_PopulationManager& operator=( const TER_PopulationManager& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TER_PopulationConcentrationManager concentrationManager_;
    TER_PopulationFlowManager          flowManager_;
    //@}
};

#endif // __TER_PopulationManager_h_
