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

#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgUrbanCreation;
}

namespace urban
{
    class Model;
}

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
    // $$$ FDS 2009-01-11: inherit urban::Model to remove private aggregated model
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controller& controller ); 
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Create( const MsgsSimToClient::MsgUrbanCreation& message );
    void Purge();
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
    kernel::Controller& controller_;
    std::auto_ptr< urban::Model > model_;
    //@}
};

#endif // __UrbanModel_h_
