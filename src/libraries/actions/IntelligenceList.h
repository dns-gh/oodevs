// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterIntelligenceList_h_
#define __ActionParameterIntelligenceList_h_

#include "game_asn/Simulation.h"
#include "Parameter.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Formation_ABC;
    class FormationLevels;
    class Controller;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  IntelligenceList
    @brief  IntelligenceList
*/
// Created: SBO 2007-10-23
// =============================================================================
class IntelligenceList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IntelligenceList( const kernel::OrderParameter& parameter );
             IntelligenceList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis
                                            , const tools::Resolver_ABC< kernel::Formation_ABC >& resolver
                                            , const kernel::FormationLevels& levels, kernel::Controller& controller );
             IntelligenceList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_IntelligenceList& asn 
                                            , const tools::Resolver_ABC< kernel::Formation_ABC >& resolver
                                            , const kernel::FormationLevels& levels, kernel::Controller& controller );
    virtual ~IntelligenceList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceList( const IntelligenceList& );            //!< Copy constructor
    IntelligenceList& operator=( const IntelligenceList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadIntelligence( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter
                         , const tools::Resolver_ABC< kernel::Formation_ABC >& resolver
                         , const kernel::FormationLevels& levels, kernel::Controller& controller );
    //@}
};

    }
}

#endif // __ActionParameterIntelligenceList_h_
