// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamInhabitant_h_
#define __ParamInhabitant_h_

#include "EntityParameter.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamInhabitant
    @brief  ParamInhabitant
*/
// Created: FPO 2011-05-30
// =============================================================================
class ParamInhabitant : public EntityParameter< kernel::Inhabitant_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamInhabitant( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller );
             ParamInhabitant( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Inhabitant_ABC& entity, kernel::Controller& controller );
    virtual ~ParamInhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetName( const QString& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamInhabitant( const ParamInhabitant& );            //!< Copy constructor
    ParamInhabitant& operator=( const ParamInhabitant& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    //@}
};

    }
}

#endif // __ParamInhabitant_h_
