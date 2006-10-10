// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "DIN_Types.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class InstanciationComplete;
}

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public kernel::Team_ABC
           , public kernel::Extension_ABC
           , public kernel::Updatable_ABC< kernel::InstanciationComplete >
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( uint id, DIN::DIN_Input& input, kernel::Controller& controller );
    virtual ~Team();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    //@}

private:
    //! @name Copy / Assignement
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    QString name_;
    unsigned long id_;
    //@}
};

#endif // __Team_h_
