// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationDetections_h_
#define __PopulationDetections_h_

#include "DIN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Controller;
class Population;
class PopulationPart_ABC;

// =============================================================================
/** @class  PopulationDetections
    @brief  PopulationDetections
*/
// Created: AGE 2006-02-27
// =============================================================================
class PopulationDetections : public Extension_ABC
                           , public Updatable_ABC< ConcentrationDetectionMessage >
                           , public Updatable_ABC< FlowDetectionMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationDetections( Controller& controller, const Resolver_ABC< Population >& resolver );
    virtual ~PopulationDetections();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationDetections( const PopulationDetections& );            //!< Copy constructor
    PopulationDetections& operator=( const PopulationDetections& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ConcentrationDetectionMessage& message );
    virtual void DoUpdate( const FlowDetectionMessage& message );
    //@}

    //! @name Types
    //@{
    typedef std::set< const PopulationPart_ABC* > T_Parts;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Population >& resolver_;
    T_Parts perceived_;
    //@}
};

#endif // __PopulationDetections_h_
