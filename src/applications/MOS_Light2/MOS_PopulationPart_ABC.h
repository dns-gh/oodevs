// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationPart_ABC_h_
#define __MOS_PopulationPart_ABC_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Population;

// =============================================================================
// Created: HME 2005-10-04
// =============================================================================
class MOS_PopulationPart_ABC
{
    MT_COPYNOTALLOWED( MOS_PopulationPart_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationPart_ABC( uint nID, const MOS_Population& parent , E_PopulationAttitude attitude = ePopulationAttitude_Calme, int persons = 0 );
    virtual ~MOS_PopulationPart_ABC();
    //@}
    
    //! @name Accessors
    //@{
    uint                    GetLivingHumans  () const;
    uint                    GetDeadHumans    () const;
    uint                    GetID            () const;
    const std::string&      GetStringAttitude() const;
    E_PopulationAttitude    GetAttitude      () const;    

    virtual const std::string& GetName() const = 0;
    virtual const MT_Vector2D& GetPos()  const = 0;
    //@}

protected:
    const MOS_Population&      parent_;
          uint                 nID_;
          int                  nLivingHumans_;
          int                  nDeadHumans_;
          E_PopulationAttitude attitude_;
};

#include "MOS_PopulationPart_ABC.inl"

#endif // __MOS_PopulationPart_ABC_h_
