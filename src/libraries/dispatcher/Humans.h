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

namespace sword
{
    enum EnumHumanRank;
    class HumanDotations_HumanDotation;
}

namespace dispatcher
{

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
    explicit Humans( const sword::HumanDotations_HumanDotation& asnMsg );
    virtual ~Humans();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::HumanDotations_HumanDotation& asnMsg );
    void Send  ( sword::HumanDotations_HumanDotation& asnMsg ) const ;
    //@}

public:
    sword::EnumHumanRank nRank_;
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
