// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterIntelligence_h_
#define __ActionParameterIntelligence_h_

#include "game_asn/Simulation.h"
#include "Entity.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Formation_ABC;
    class FormationLevels;
    class Intelligence_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Intelligence
    @brief  Intelligence
*/
// Created: SBO 2007-10-23
// =============================================================================
class Intelligence : public Entity< kernel::Intelligence_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Intelligence( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller );
             Intelligence( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis
                                        , const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver
                                        , const kernel::FormationLevels& levels, kernel::Controller& controller );
             Intelligence( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter
                                        , const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver
                                        , const kernel::FormationLevels& levels
                                        , const ASN1T_Intelligence& asn, kernel::Controller& controller );
    virtual ~Intelligence();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_Intelligence& asn ) const;
    void Clean( ASN1T_Intelligence& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void CommitToChildren();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligence( const Intelligence& );            //!< Copy constructor
    Intelligence& operator=( const Intelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver, const kernel::FormationLevels& levels );
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

    }
}

#endif // __ActionParameterIntelligence_h_
