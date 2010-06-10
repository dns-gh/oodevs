// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_UrbanModel_h_
#define __preparation_UrbanModel_h_

#include "tools/Resolver.h"
#include <urban/Model.h>

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public urban::Model
                 , public tools::Resolver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanModel( kernel::Controller& controller ); 
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    bool Load( const std::string& directoryPath, urban::WorldParameters& world );
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanModel( const UrbanModel& );            //!< Copy constructor
    UrbanModel& operator=( const UrbanModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __preparation_UrbanModel_h_
