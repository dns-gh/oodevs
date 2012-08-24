// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_kernel/Team.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionVariant;
}

namespace xml
{
    class xostream;
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::Team
           , public kernel::Extension_ABC
           , public tools::Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controllers& controllers, IdManager& idManager );
             Team( xml::xistream& xis, kernel::Controllers& controllers, IdManager& idManager );
    virtual ~Team();
    //@}
};

#endif // __Team_h_
