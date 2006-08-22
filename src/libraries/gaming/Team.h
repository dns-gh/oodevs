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
}

class KnowledgeGroupFactory_ABC;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public kernel::Team_ABC
           , public kernel::Extension_ABC
           , public kernel::Updatable_ABC< KnowledgeGroupCreationMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( uint id, DIN::DIN_Input& input, kernel::Controller& controller, KnowledgeGroupFactory_ABC& factory );
    virtual ~Team();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual std::string GetName() const;
    //@}

private:
    //! @name Copy / Assignement
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const KnowledgeGroupCreationMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    KnowledgeGroupFactory_ABC& factory_;
    std::string name_;
    unsigned long id_;
    //@}
};

#endif // __Team_h_
