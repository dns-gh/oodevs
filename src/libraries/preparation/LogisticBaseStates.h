// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticBaseStates_h__
#define __LogisticBaseStates_h__

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
/** @class  LogisticBaseStates
    @brief  LogisticBaseStates
*/
// Created: AHC 2010-09-29
// =============================================================================
class LogisticBaseStates : public LogisticHierarchies< LogisticBaseSuperior, kernel::LogisticBaseHierarchies >
                   , public tools::Resolver< Dotation >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticBaseStates( kernel::Controller& controller, kernel::Entity_ABC& entity,
                 const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~LogisticBaseStates();
    //@}

    //! @name Operations
    //@{
    void SetSuperior( const LogisticBaseSuperior& superior );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticBaseStates( const LogisticBaseStates& );            //!< Copy constructor
    LogisticBaseStates& operator=( const LogisticBaseStates& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner );
    virtual void Load( xml::xistream& xis );
    void ReadDotation( xml::xistream& xis );
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

#endif // __LogisticBaseStates_h__
