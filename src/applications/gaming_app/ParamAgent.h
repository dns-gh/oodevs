// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgent_h_
#define __ParamAgent_h_

#include "EntityParameter.h"
#include "clients_kernel/Agent_ABC.h"

// =============================================================================
/** @class  ParamAgent
    @brief  ParamAgent
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgent : public EntityParameter< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgent( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller );
             ParamAgent( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Agent_ABC& entity, kernel::Controller& controller );
    virtual ~ParamAgent();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgent( const ParamAgent& );            //!< Copy constructor
    ParamAgent& operator=( const ParamAgent& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    //@}
};

#endif // __ParamAgent_h_
