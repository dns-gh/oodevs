// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyStates_h_
#define __SupplyStates_h_

#include "Types.h"
#include "LogisticHierarchies.h"
#include "tools/Resolver.h"

namespace kernel
{
    class DotationType;
}

class Dotation;
class DotationsItem;

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: SBO 2006-10-24
// =============================================================================
class SupplyStates : public LogisticHierarchies< SupplySuperior, kernel::SupplyHierarchies >
                   , public tools::Resolver< Dotation >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( kernel::Controller& controller, kernel::Entity_ABC& entity, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@
    void SetSuperior( const SupplySuperior& automat );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyStates( const SupplyStates& );            //!< Copy constructor
    SupplyStates& operator=( const SupplyStates& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Load( xml::xistream& xis );
    void ReadDotation( xml::xistream& xis );
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner );
    virtual void SerializeQuotas( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver_;
    DotationsItem* item_;
    //@}
};

#endif // __SupplyStates_h_
