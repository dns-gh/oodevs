// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "Types.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

namespace kernel
{
    class Controller;
    class ObjectType;
    class DotationType;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object : public kernel::EntityImplementation< kernel::Object_ABC >
             , public kernel::Extension_ABC
             , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Object( const MsgsSimToClient::MsgObjectCreation& message, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                     const tools::Resolver_ABC< kernel::ObjectType, std::string >& typeResolver );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    const kernel::ObjectType& GetType() const;
    virtual QString GetTypeName() const;
    bool IsReservedObstacle() const;
    bool IsReservedObstacleActivated() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::ObjectType& type_;

    kernel::OptionalValue< float > rConstructionPercentage_;
    kernel::OptionalValue< float > rValorizationPercentage_;
    kernel::OptionalValue< float > rBypassConstructionPercentage_;
    kernel::OptionalValue< E_DemolitionTargetType > obstacleType_;
    kernel::OptionalValue< bool > reservedObstacleActivated_;
    //@}

    //! @name Member data
    //@{
    MsgLocation::Geometry nTypeLocalisation_;
    kernel::DotationType* construction_;
    kernel::DotationType* valorization_;
    kernel::OptionalValue< unsigned int > nDotationConstruction_;
    kernel::OptionalValue< unsigned int > nDotationValorization_;
    //@}
};

#endif // __Object_h_
