// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentDetections_h_
#define __AgentDetections_h_

#include "DIN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"
#include "Attr_Def.h"

class Controller;
class Agent;

// =============================================================================
/** @class  AgentDetections
    @brief  AgentDetections
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentDetections : public Extension_ABC
                      , public Updatable_ABC< DetectionMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentDetections( Controller& controller, const Resolver_ABC< Agent >& resolver );
    virtual ~AgentDetections();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentDetections( const AgentDetections& );            //!< Copy constructor
    AgentDetections& operator=( const AgentDetections& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Agent*, E_UnitVisType > T_AgentDetections;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const DetectionMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;

    T_AgentDetections detections_;
    //@}
};

#endif // __AgentDetections_h_
