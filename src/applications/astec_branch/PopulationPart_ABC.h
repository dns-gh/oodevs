// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPart_ABC_h_
#define __PopulationPart_ABC_h_

#include "Types.h"
#include "ASN_Types.h"

class Population;

// =============================================================================
// Created: HME 2005-10-04
// =============================================================================
class PopulationPart_ABC
{
    MT_COPYNOTALLOWED( PopulationPart_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPart_ABC( uint nID, const Population& parent , E_PopulationAttitude attitude = ePopulationAttitude_Calme, int persons = 0 );
    virtual ~PopulationPart_ABC();
    //@}
    
    //! @name Accessors
    //@{
    uint                    GetLivingHumans  () const;
    uint                    GetDeadHumans    () const;
    uint                    GetID            () const;
    const std::string&      GetStringAttitude() const;
    E_PopulationAttitude    GetAttitude      () const;
    const Population&   GetPopulation    () const;

    virtual const std::string& GetName   () const = 0;
    virtual const MT_Vector2D& GetPos    () const = 0;
    virtual       MT_Float     GetDensity() const;
    //@}

protected:
    const Population&      parent_;
          uint                 nID_;
          int                  nLivingHumans_;
          int                  nDeadHumans_;
          E_PopulationAttitude attitude_;
          MT_Float             rDensity_; // living density
};

#include "PopulationPart_ABC.inl"

#endif // __PopulationPart_ABC_h_
