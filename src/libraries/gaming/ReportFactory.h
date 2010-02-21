// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ReportFactory_h_
#define __ReportFactory_h_

#include "tools/Resolver.h"
#include "protocol/Protocol.h"

using namespace Common;


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

    Report* CreateReport( const kernel::Entity_ABC& agent, const MsgsSimToClient::MsgReport&  message ) const;
    Report* CreateTrace ( const kernel::Entity_ABC& agent, const MsgsSimToClient::MsgTrace& message ) const;
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
    QString RenderParameter( const Common::MsgMissionParameter& value ) const;
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
