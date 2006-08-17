// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConcentrationDetections_h_
#define __ConcentrationDetections_h_

#include "DIN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

class Controller;
class Population_ABC;
class PopulationConcentration;

// =============================================================================
/** @class  ConcentrationDetections
    @brief  ConcentrationDetections
*/
// Created: AGE 2006-02-14
// =============================================================================
class ConcentrationDetections : public Extension_ABC
                              , public Updatable_ABC< ConcentrationDetectionMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             ConcentrationDetections( Controller& controller, const Resolver_ABC< Population_ABC >& resolver );
    virtual ~ConcentrationDetections();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConcentrationDetections( const ConcentrationDetections& );            //!< Copy constructor
    ConcentrationDetections& operator=( const ConcentrationDetections& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::set< const PopulationConcentration* > T_Concentrations;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ConcentrationDetectionMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Population_ABC >& resolver_;

    T_Concentrations perceivedConcentrations_;
    //@}
};

#endif // __ConcentrationDetections_h_
