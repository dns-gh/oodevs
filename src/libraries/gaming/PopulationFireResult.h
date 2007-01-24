// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFireResult_h_
#define __PopulationFireResult_h_

#include "network/Simulation_Asn.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Population_ABC;
}

// =============================================================================
/** @class  PopulationFireResult
    @brief  PopulationFireResult
*/
// Created: AGE 2006-03-10
// =============================================================================
class PopulationFireResult
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationFireResult( const ASN1T_FireDamagesPopulation& message, const kernel::Resolver_ABC< kernel::Population_ABC >& resolver );
    virtual ~PopulationFireResult();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFireResult( const PopulationFireResult& );            //!< Copy constructor
    PopulationFireResult& operator=( const PopulationFireResult& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    const kernel::Population_ABC& target_;
    unsigned int deadPeople_;
    //@}
};

#endif // __PopulationFireResult_h_
