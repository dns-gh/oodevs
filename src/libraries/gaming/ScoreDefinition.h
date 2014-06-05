// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreDefinition_h_
#define __ScoreDefinition_h_

#include "IndicatorDefinition_ABC.h"

namespace indicators
{
    class ElementFactory_ABC;
    class Gauge;
    class GaugeFactory_ABC;
    class Primitives;
    class Variables;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ScoreDefinition
    @brief  ScoreDefinition
*/
// Created: SBO 2009-04-29
// =============================================================================
class ScoreDefinition : public IndicatorDefinition_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoreDefinition( xml::xistream& xis, const indicators::Primitives& primitives, const indicators::GaugeFactory_ABC& factory );
    virtual ~ScoreDefinition();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual QString GetDisplayName() const;
    virtual std::string Commit( const T_Parameters& parameters ) const;
    indicators::Gauge* CreateGauge() const;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string formula_;
    std::unique_ptr< indicators::Gauge > gauge_;
    std::unique_ptr< indicators::Variables > variables_;
    std::unique_ptr< indicators::ElementFactory_ABC > elementFactory_;
    //@}
};

#endif // __ScoreDefinition_h_
