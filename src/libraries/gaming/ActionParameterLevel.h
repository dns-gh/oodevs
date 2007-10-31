// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLevel_h_
#define __ActionParameterLevel_h_

#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class HierarchyLevel_ABC;
    class FormationLevels;
}

// =============================================================================
/** @class  ActionParameterLevel
    @brief  ActionParameterLevel
*/
// Created: SBO 2007-10-23
// =============================================================================
class ActionParameterLevel : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLevel( const kernel::OrderParameter& parameter, const kernel::HierarchyLevel_ABC& level );
             ActionParameterLevel( const kernel::OrderParameter& parameter, const ASN1T_EnumNatureLevel& asn, const kernel::FormationLevels& levels );
             ActionParameterLevel( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::FormationLevels& levels );
    virtual ~ActionParameterLevel();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_EnumNatureLevel& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLevel( const ActionParameterLevel& );            //!< Copy constructor
    ActionParameterLevel& operator=( const ActionParameterLevel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::HierarchyLevel_ABC& level_;
    //@}
};

#endif // __ActionParameterLevel_h_
