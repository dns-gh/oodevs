// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterKarma_h_
#define __ActionParameterKarma_h_

#include "Parameter.h"
#include <boost/function.hpp>

namespace Common
{
    enum EnumDiplomacy;
}

namespace kernel
{
    class Karma;
    class Entity_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Karma
    @brief  Karma
*/
// Created: SBO 2007-10-24
// =============================================================================
class Karma : public Parameter< QString >
{
public:
    //! @name Functors
    //@{
    typedef boost::function< void ( const Common::EnumDiplomacy& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Karma( const kernel::OrderParameter& parameter, const kernel::Karma& karma, const kernel::Entity_ABC& parent );
             Karma( const kernel::OrderParameter& parameter, const Common::EnumDiplomacy& message );
             Karma( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~Karma();
    //@}

    //! @name Operations
    //@{
    void CommitTo( T_Setter setter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Karma( const Karma& );            //!< Copy constructor
    Karma& operator=( const Karma& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Karma& karma_;
    //@}
};
    }
}

#endif // __ActionParameterKarma_h_
