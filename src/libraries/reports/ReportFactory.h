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

#include <tools/Resolver.h>
#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
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
    class DotationType;
    class Entity_ABC;
    class EquipmentType;
    class Time_ABC;
}

namespace sword
{
    class DateTime;
    class MissionParameter;
    class MissionParameter_Value;
    class Report;
    class Trace;
}

class Stages;
class Report;
class RcEntityResolver_ABC;
class ReportTemplate;

// =============================================================================
/** @class  ReportFactory
    @brief  Report factory
*/
// Created: SBO 2006-12-07
// =============================================================================
class ReportFactory : public tools::Resolver< ReportTemplate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportFactory( const RcEntityResolver_ABC& rcResolver,
                            const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                            const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver,
                            const kernel::Time_ABC* time );
    virtual ~ReportFactory();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();

    boost::shared_ptr< Report > CreateReport( const kernel::Entity_ABC& entity, const sword::Report& message ) const;
    Report* CreateTrace( const kernel::Entity_ABC& entity, const sword::Trace& message ) const;
    std::string FormatReport( const kernel::Entity_ABC* entity, const sword::Report& message ) const;
    QDateTime GetTime( const sword::DateTime& d ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportFactory( const ReportFactory& );            //!< Copy constructor
    ReportFactory& operator=( const ReportFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadReport( xml::xistream& xis );
    friend class ReportTemplate;
    QString RenderParameter( const kernel::Entity_ABC* entity, const sword::MissionParameter_Value& value ) const;
    //@}

private:
    //! @name Member data
    //@{
    const RcEntityResolver_ABC& rcResolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver_;
    const kernel::Time_ABC* time_;
    std::unique_ptr< Stages > stages_;
    //@}
};

#endif // __ReportFactory_h_
