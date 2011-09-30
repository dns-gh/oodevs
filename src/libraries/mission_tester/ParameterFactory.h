// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParameterFactory_h_
#define __ParameterFactory_h_

#include "Model.h"
#include <boost/noncopyable.hpp>
#include <vector>

namespace actions
{
    class DotationType;
    class Parameter_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Controller;
    class EntityResolver_ABC;
}

namespace xml
{
    class xistream;
}

namespace mission_tester
{
    struct Coordinates
    {
        Coordinates( const std::string& coord )
            : x     ( ( int( coord [ 5 ] ) - 48 ) * 10000 + ( int ( coord [ 6 ] ) - 48 ) * 1000 + ( int ( coord [ 7 ] ) - 48 ) * 100 + ( int ( coord [ 8 ] ) - 48 ) * 10 + ( int ( coord [ 9 ] ) - 48 ) )
            , y     ( ( int( coord [ 10 ] ) - 48 ) * 10000 + ( int ( coord [ 11 ] ) - 48 ) * 1000 + ( int ( coord [ 12 ] ) - 48 ) * 100 + ( int ( coord [ 13 ] ) - 48 ) * 10 + ( int ( coord [ 14 ] ) - 48 ) )
            , alphaX( coord[ 3 ] )
            , alphaY( coord[ 4 ] )
        {
            prefix = coord[0];
            prefix += coord[1];
            prefix += coord[2];
        }
        unsigned int x;
        unsigned int y;
        char alphaX;
        char alphaY;
        std::string prefix;
    };

// =============================================================================
/** @class  ParameterFactory
    @brief  ParameterFactory
*/
// Created: PHC 2011-04-07
// =============================================================================
class ParameterFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ParameterFactory( const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller,
                               const kernel::EntityResolver_ABC& entityResolver, xml::xistream& xis, const Model& model );
    virtual ~ParameterFactory();
    //@}

    //! @name Operations
    //@{
    std::auto_ptr< actions::Parameter_ABC > CreateParameter( const kernel::OrderParameter& parameter ) const;
    //@}

private:
    //! @name Helpers
    //@{
    std::auto_ptr< actions::Parameter_ABC > CreateMaintenancePrioritiesParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateMedicalPrioritiesParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateDateTimeParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateResourceTypeParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateNatureAtlasParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePlannedWorkParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateObjectKnowledgeParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateAgentKnowledgeParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateCrowdKnowledgeParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePhaselineParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateLimitParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePointParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePathParameter( const kernel::OrderParameter& parameter/*, const Entity_ABC& target*/ ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateAgentParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateAutomatParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePolygonParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateLocationCompositeParameter( const kernel::OrderParameter& parameter ) const;
	//@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    const kernel::EntityResolver_ABC& entityResolver_;
    Coordinates upperLeft_;
    Coordinates lowerRight_;
    const Model& model_;
    //@}
};

}

#endif // __ParameterFactory_h_
