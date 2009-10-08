// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportFactory_h_
#define __ReportFactory_h_

#include "tools/Resolver.h"
#include "game_asn/Simulation.h"

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Entity_ABC;
    class DotationType;
    class EquipmentType;
}

class Simulation;
class Report;
class RcEntityResolver_ABC;
class ReportTemplate;

// =============================================================================
/** @class  ReportFactory
    @brief  ReportFactory
*/
// Created: SBO 2006-12-07
// =============================================================================
class ReportFactory : public tools::Resolver< ReportTemplate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ReportFactory( const RcEntityResolver_ABC& rcResolver
                          , const tools::Resolver_ABC< kernel::DotationType >& dotationResolver
                          , const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver
                          , const Simulation& simulation );
    virtual ~ReportFactory();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();

    Report* CreateReport( const kernel::Entity_ABC& agent, const ASN1T_MsgReport&  asnMsg ) const;
    Report* CreateTrace ( const kernel::Entity_ABC& agent, const ASN1T_MsgTrace& asnMsg ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ReportFactory( const ReportFactory& );            //!< Copy constructor
    ReportFactory& operator=( const ReportFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadReport( xml::xistream& xis );
    friend class ReportTemplate;
    QString RenderParameter( const ASN1T_MissionParameter& value ) const;
    //@}

private:
    //! @name Member data
    //@{
    const RcEntityResolver_ABC&                          rcResolver_;
    const tools::Resolver_ABC< kernel::DotationType >&  dotationResolver_;
    const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver_;
    const Simulation& simulation_;
    //@}
};

#endif // __ReportFactory_h_
