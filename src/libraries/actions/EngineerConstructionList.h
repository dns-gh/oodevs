// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ActionsParameters_EngineerConstructionList_h_
#define __ActionsParameters_EngineerConstructionList_h_

#include "game_asn/Simulation.h"
#include "Parameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class ObjectType;
    class Automat_ABC;
    class Controller;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  EngineerConstructionList
    @brief  EngineerConstructionList
*/
// Created: JCR 2008-11-03
// =============================================================================
class EngineerConstructionList : public Parameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EngineerConstructionList( const kernel::OrderParameter& parameter );
             EngineerConstructionList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, const ASN1T_PlannedWorkList& asn, kernel::Controller& controller );
             EngineerConstructionList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller );
    virtual ~EngineerConstructionList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EngineerConstructionList( const EngineerConstructionList& );            //!< Copy constructor
    EngineerConstructionList& operator=( const EngineerConstructionList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadEngineerConstruction( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, kernel::Controller& controller );
    //@}
};

} }

#endif // __ActionsParameters_EngineerConstructionList_h_
