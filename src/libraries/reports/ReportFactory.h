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
#include "game_asn/Simulation.h"

#pragma warning( push )
#pragma warning( disable : 4996 )
#include <qstring.h>
#include <qdatetime.h>
#pragma warning( pop )

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
    class Time_ABC;
}

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
                          , const kernel::Resolver_ABC< kernel::EquipmentType >& equipmentResolver
                          , const kernel::Time_ABC* time );
    virtual ~ReportFactory();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();

    Report* CreateReport( const kernel::Entity_ABC& agent, const ASN1T_MsgReport&  asnMsg ) const;
    Report* CreateTrace ( const kernel::Entity_ABC& agent, const ASN1T_MsgTrace& asnMsg ) const;
    std::string FormatReport( const ASN1T_MsgReport& asn ) const;
    QDateTime GetTime( const ASN1T_DateTime& d ) const;
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
    const kernel::Time_ABC*                              time_;
    //@}
};

#endif // __ReportFactory_h_
