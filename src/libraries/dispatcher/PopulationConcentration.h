 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_h_
#define __PopulationConcentration_h_

#include "network/Simulation_Asn.h"
#include "Position.h"

namespace dispatcher
{
class Publisher_ABC;
class Model;
class Population;

// =============================================================================
/** @class  PopulationConcentration
    @brief  PopulationConcentration
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationConcentration
{
public:
    //! @name Constructors/Destructor
    //@{
     PopulationConcentration( Model& model, Population& population, const ASN1T_MsgPopulationConcentrationCreation& msg );
    ~PopulationConcentration();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationConcentrationUpdate& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignement operator
    //@}

private:
          Population&   population_;
    const unsigned long nID_;
    const Position      position_;

    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    ASN1T_EnumPopulationAttitude nAttitude_;    
};

}

#include "PopulationConcentration.inl"

#endif // __PopulationConcentration_h_
