 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Humans_h_
#define __Humans_h_

#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  Humans
    @brief  Humans
*/
// Created: NLD 2006-09-19
// =============================================================================
class Humans
{
public:
    //! @name Constructors/Destructor
    //@{
     Humans( const Model& model, const ASN1T_HumanDotations& asnMsg );
    ~Humans();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_HumanDotations& asnMsg );
    void Send  ( ASN1T_HumanDotations& asnMsg ) const ;
    //@}

public:
    ASN1T_EnumHumanRank nRank_;
    unsigned int        nNbrTotal_;
    unsigned int        nNbrOperational_;
    unsigned int        nNbrDead_;
    unsigned int        nNbrWounded_;
    unsigned int        nNbrMentalDiseased_;
    unsigned int        nNbrNBC_;
    unsigned int        nNbrInLogisticMedical_;
    unsigned int        nNbrInLogisticMaintenance_;
};

}

#endif // __Humans_h_
