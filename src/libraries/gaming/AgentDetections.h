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
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "Attr_Def.h"

class Controller;
class Agent_ABC;
class Team_ABC;

// =============================================================================
/** @class  AgentDetections
    @brief  AgentDetections
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentDetections : public Extension_ABC
                      , public Updatable_ABC< DetectionMessage >
                      , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentDetections( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Team_ABC& team );
    virtual ~AgentDetections();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentDetections( const AgentDetections& );            //!< Copy constructor
    AgentDetections& operator=( const AgentDetections& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Agent_ABC*, E_UnitVisType > T_AgentDetections;
    typedef T_AgentDetections::const_iterator     CIT_AgentDetections;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const DetectionMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent_ABC >& resolver_;
    const Team_ABC& team_;

    T_AgentDetections detections_;
    //@}
};

#endif // __AgentDetections_h_
