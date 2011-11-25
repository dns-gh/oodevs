// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgent_h_
#define __ParamAgent_h_

#include "EntityParameter.h"
#include "clients_kernel/Agent_ABC.h"

namespace actions
{
    namespace gui
    {

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
    void SetName( const QString& name );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    //@}
};

    }
}

#endif // __ParamAgent_h_
