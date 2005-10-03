// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MIL_PopulationFlow_h_
#define __MIL_PopulationFlow_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"

class MIL_Population;
class MIL_PopulationConcentration;
class DEC_Population_Path;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationFlow
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationFlow( const MIL_Population& population, MIL_PopulationConcentration& sourceConcentration );
    ~MIL_PopulationFlow();
    //@}

    //! @name Operations
    //@{
    void Move( const MT_Vector2D& destination );
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationFlow( const MIL_PopulationFlow& );            //!< Copy constructor
    MIL_PopulationFlow& operator=( const MIL_PopulationFlow& ); //!< Assignement operator
    //@}

private:
    const MIL_Population&              population_;
    const uint                         nID_;

          MT_Vector2D                  destination_;
          DEC_Population_Path*         pCurrentPath_;
          bool                         bHasMoved_;

          MIL_PopulationConcentration* pSourceConcentration_;
          MIL_PopulationConcentration* pDestConcentration_;

public:
    static MIL_MOSIDManager idManager_;
};

#include "MIL_PopulationFlow.inl"

#endif // __MIL_PopulationFlow_h_
