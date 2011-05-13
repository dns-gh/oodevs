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
#include "tools/Resolver_ABC.h"
#include <boost/function.hpp>

namespace sword
{
    enum EnumNatureLevel;
}

namespace kernel
{
    class HierarchyLevel_ABC;
    class FormationLevels;
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
    typedef boost::function< void ( const sword::EnumNatureLevel& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Level( const kernel::OrderParameter& parameter, const kernel::HierarchyLevel_ABC& level );
             Level( const kernel::OrderParameter& parameter, const sword::EnumNatureLevel& message, const kernel::FormationLevels& levels );
             Level( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::FormationLevels& levels );
    virtual ~Level();
    //@}

    //! @name Operations
    //@{
    void CommitTo( T_Setter setter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Level( const Level& );            //!< Copy constructor
    Level& operator=( const Level& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::HierarchyLevel_ABC& level_;
    //@}
};

    }
}

#endif // __ActionParameterLevel_h_
