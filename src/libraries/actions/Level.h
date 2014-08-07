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

#include "Parameter.h"
#include "ENT/ENT_Enums.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    enum EnumNatureLevel;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Level
    @brief  Level
*/
// Created: SBO 2007-10-23
// =============================================================================
class Level : public Parameter< QString >

{
public:
    //! @name Functors
    //@{
    typedef std::function< void ( const sword::EnumNatureLevel& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Level( const kernel::OrderParameter& parameter, E_NatureLevel level );
             Level( const kernel::OrderParameter& parameter, const sword::EnumNatureLevel& message );
    virtual ~Level();
    //@}

    //! @name Operations
    //@{
    void CommitTo( T_Setter setter ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    E_NatureLevel level_;
    //@}
};

    }
}

#endif // __ActionParameterLevel_h_
