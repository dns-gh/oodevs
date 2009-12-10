// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanModel_h_
#define __UrbanModel_h_

#include "game_asn/Simulation.h"
#include "tools/Resolver.h"
#include "clients_kernel/Entity_ABC.h"

namespace urban
{
    class Model;
    class StaticModel;
}

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanModel( kernel::Controller& controller, urban::StaticModel& staticModel ); 
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Create( const ASN1T_MsgUrbanCreation& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanModel( const UrbanModel& );            //!< Copy constructor
    UrbanModel& operator=( const UrbanModel& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    urban::StaticModel& staticModel_;
    urban::Model&       model_;
    kernel::Controller& controller_;
    //@}
};

#endif // __UrbanModel_h_
