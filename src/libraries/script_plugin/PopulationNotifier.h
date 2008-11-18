// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __PopulationNotifier_h_
#define __PopulationNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/simulation.h"

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Population;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  PopulationNotifier
    @brief  PopulationNotifier
*/
// Created: SBO 2008-11-18
// =============================================================================
class PopulationNotifier : public kernel::Extension_ABC
                         , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationNotifier( kernel::Controller& controller, const dispatcher::Population& population );
    virtual ~PopulationNotifier();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationNotifier( const PopulationNotifier& );            //!< Copy constructor
    PopulationNotifier& operator=( const PopulationNotifier& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Population& population_;
    //@}
};

}
}

#endif // __PopulationNotifier_h_
