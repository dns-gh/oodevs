// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FlowDetections_h_
#define __FlowDetections_h_

#include "DIN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Controller;
class Population;
class PopulationFlow;

// =============================================================================
/** @class  FlowDetections
    @brief  FlowDetections
*/
// Created: AGE 2006-02-14
// =============================================================================
class FlowDetections : public Extension_ABC
                     , public Updatable_ABC< FlowDetectionMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             FlowDetections( Controller& controller, const Resolver_ABC< Population >& resolver );
    virtual ~FlowDetections();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FlowDetections( const FlowDetections& );            //!< Copy constructor
    FlowDetections& operator=( const FlowDetections& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< MT_Vector2D >                     T_FlowChunk;
    typedef std::map< const PopulationFlow*, T_FlowChunk > T_Flows;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const FlowDetectionMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Population >& resolver_;

    T_Flows perceivedFlows_;
    //@}
};

#endif // __FlowDetections_h_
