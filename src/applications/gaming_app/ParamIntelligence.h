// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamIntelligence_h_
#define __ParamIntelligence_h_

#include "clients_kernel/Intelligence_ABC.h"
#include "EntityParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ParamIntelligence
    @brief  ParamIntelligence
*/
// Created: SBO 2007-10-24
// =============================================================================
class ParamIntelligence : public EntityParameter< kernel::Intelligence_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamIntelligence( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Intelligence_ABC& entity, kernel::Controller& controller );
    virtual ~ParamIntelligence();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamIntelligence( const ParamIntelligence& );            //!< Copy constructor
    ParamIntelligence& operator=( const ParamIntelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __ParamIntelligence_h_
