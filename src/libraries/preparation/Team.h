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

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::EntityImplementation< kernel::Team_ABC >
           , public kernel::Extension_ABC
           , public kernel::Serializable_ABC
           , public tools::Observer_ABC
           , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controllers& controllers, tools::IdManager& idManager );
             Team( xml::xistream& xis, kernel::Controllers& controllers, tools::IdManager& idManager );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Data Member
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __Team_h_
