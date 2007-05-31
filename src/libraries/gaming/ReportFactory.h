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

#include "clients_kernel/Resolver.h"
#include "DIN_Types.h"
#include "game_asn/Asn.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Entity_ABC;
    class DotationType;
    class EquipmentType;
    class ExerciseConfig;
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
class ReportFactory : public kernel::Resolver< ReportTemplate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ReportFactory( const RcEntityResolver_ABC& rcResolver
                          , const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver
                          , const kernel::Resolver_ABC< kernel::EquipmentType >& equipmentResolver );
    virtual ~ReportFactory();
    //@}

    //! @name Operations
    //@{
    void Load( const kernel::ExerciseConfig& config );
    void Purge();

    Report* CreateReport( const kernel::Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgCR&  asnMsg ) const;
    Report* CreateTrace ( const kernel::Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgTrace& asnMsg ) const;
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
    const kernel::Resolver_ABC< kernel::DotationType >&  dotationResolver_;
    const kernel::Resolver_ABC< kernel::EquipmentType >& equipmentResolver_;
    //@}
};

#endif // __ReportFactory_h_
