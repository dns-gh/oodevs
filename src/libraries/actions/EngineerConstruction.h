// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameter_EngineerConstruction_h_
#define __ActionParameter_EngineerConstruction_h_

#include "Parameter.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class PlannedWork;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class ObjectType;
    class Automat_ABC;
    class Controller;
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  EngineerConstruction
    @brief  EngineerConstruction
*/
// Created: JCR 2008-11-03
// =============================================================================
class EngineerConstruction : public Parameter< std::string >
{
    public:
    //! @name Constructors/Destructor
    //@{
             EngineerConstruction( const kernel::OrderParameter& parameter );
             EngineerConstruction( const kernel::OrderParameter& parameter, const kernel::ObjectType& type );
             EngineerConstruction( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, const sword::PlannedWork& message, kernel::Controller& controller );
             EngineerConstruction( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller );
             EngineerConstruction( const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller );
    virtual ~EngineerConstruction();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual bool IsSet() const;
    //@}

    //! @name Commiters
    //@{
    void CommitTo( sword::PlannedWork& message ) const;
    void Clean( sword::PlannedWork& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller );
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    void SetParameters( const sword::PlannedWork& message, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ObjectType* type_;
    //@}
};

}
}

#endif // __ActionParameter_EngineerConstruction_h_
