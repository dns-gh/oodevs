// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "HospitalUpdateReport.h"
#include "dispatcher/Object.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace plugins::edxl;

namespace
{
    template< typename T >
    class manipulator
    {
    public:
        explicit manipulator( T functor ) : functor_ ( functor ) {}

        friend xml::xostream& operator <<( xml::xostream& xos, const manipulator< T >& m )
        {
            m.functor_( xos );
            return xos;
        }
    private:
        T functor_;
    };

    template< typename T >
    manipulator< T > xml_bind( T value )
    {
        return manipulator< T >( value );
    }

    std::string CurrentTime()
    {
        return bpt::to_iso_extended_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport constructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
HospitalUpdateReport::HospitalUpdateReport( const std::string& name, const Common::ObjectAttributeMedicalTreatment& medical, const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver )
    : xos_ ( new xml::xostringstream() )
    , resolver_ ( resolver )
{
    BuildReport( name, medical );
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::BuildReport
// Created: JCR 2010-06-09
// -----------------------------------------------------------------------------
void HospitalUpdateReport::BuildReport( const std::string& name, const Common::ObjectAttributeMedicalTreatment& medical )
{
    *xos_ << xml::start( "Hospital" )
            << xml::start( "OrganizationInformation" )
                << xml::start( "OrganizationID" ) << medical.external_reference_id() << xml::end
                << xml::start( "OrganizationName" ) << name << xml::end
            << xml::end
            << xml::start( "HospitalBedCapacityStatus" )
                << xml_bind( boost::bind( &HospitalUpdateReport::UpdateCapacityStatus, this, _1, boost::cref( medical ) ) )
            << xml::end
            << xml::start( "HospitalFacilityStatus" )
                << xml_bind( boost::bind( &HospitalUpdateReport::UpdateFacilityStatus, this, _1, boost::cref( medical ) ) )
            << xml::end
            << xml::start( "LastUpdateTime" ) << CurrentTime() << xml::end
         << xml::end;
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport destructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
HospitalUpdateReport::~HospitalUpdateReport()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::UpdateCapacityStatus
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void HospitalUpdateReport::UpdateCapacityStatus( xml::xostream& xos, const Common::ObjectAttributeMedicalTreatment& medical )
{
    // for each bed type
    for ( int i = 0; i < medical.bed_capacities_size(); ++i )
        UpdateBedStatus( xos, medical.bed_capacities( i ) );
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::UpdateBedStatus
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void HospitalUpdateReport::UpdateBedStatus( xml::xostream& xos, const Common::ObjectAttributeMedicalTreatmentBedCapacity& bed )
{
    xos << xml::start( "BedCapacity" )
            << xml::start( "BedType" ) << resolver_.Get( bed.type_id() ).GetName() << xml::end
            << xml::start( "Capacity" )
                << xml_bind( boost::bind( &HospitalUpdateReport::UpdateBedCapacity, this, _1, boost::cref( bed ) ) )
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::UpdateBedCapacity
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void HospitalUpdateReport::UpdateBedCapacity( xml::xostream& xos, const Common::ObjectAttributeMedicalTreatmentBedCapacity& bed )
{
    xos << xml::content( "CapacityStatus", ( bed.available_count() > 0 ) ? std::string( "Vacant/Available" ) : std::string( "NotAvailable" ) )
        << xml::content( "AvailableCount", bed.available_count() )
        << xml::content( "BaselineCount", bed.baseline_count() );
    
    if ( bed.has_emergency_count() )
    {
        xos << xml::content( "AdditionalCapacityCount24Hr", bed.emergency_count() )
            << xml::content( "AdditionalCapacityCount72Hr", bed.emergency_count() );
    }
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::UpdateFacilityStatus
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void HospitalUpdateReport::UpdateFacilityStatus( xml::xostream& xos, const Common::ObjectAttributeMedicalTreatment& object )
{
    xos << xml::start( "EmergencyDepartmentStatus" ) << "Normal" << xml::end; // "Advisory|Closed|On Divert|N/A"
    if( false )
    {
        xos << xml::start( "IncreasingPatientCare" ) << "IncreasingPatientCareYes" << xml::end;
        xos << xml::start( "PatientsLeftED" ) << "PatientsLeftEDYes" << xml::end;
        xos << xml::start( "DisasterProtocols" ) << "DisasterProtocolsYes" << xml::end;
        xos << xml::start( "ImplementSurgeStrategies" ) << "ImplementSurgeStrategiesYes" << xml::end;
        xos << xml::start( "InPlaceStrategies" ) << "InPlaceStrategiesYes" << xml::end;
        xos << xml::start( "AugmentedPersonnel" ) << "AugmentedPersonnelYes" << xml::end;
        xos << xml::start( "ExpandedBedCapacity" ) << "ExpandedBedCapacityYes" << xml::end;
        xos << xml::start( "EstablishAlternateCareSites" ) << "EstablishAlternateCareSitesYes" << xml::end;
        xos << xml::start( "ActivateMobileUnits" ) << "ActivateMobileUnitsYes" << xml::end;
    }
    UpdateDecon( xos, object );
    UpdateEvacuation( xos, object );
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::UpdateDecon
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void HospitalUpdateReport::UpdateDecon( xml::xostream& xos, const Common::ObjectAttributeMedicalTreatment& /*object*/ )
{
    xos << xml::start( "DeconCapacity" )
            << "Inactive" // "Open|Full|Exceeded|N/A"
         << xml::end;
}

// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::UpdateEvacuation
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void HospitalUpdateReport::UpdateEvacuation( xml::xostream& xos, const Common::ObjectAttributeMedicalTreatment& /*object*/ )
{
    xos << xml::start( "OperationalStatus" ) << "FullyOperational" << xml::end // "FullyOperational|LimitedOperation|Closed|N/A"
         << xml::start( "EvacuationStatus" ) << "NoPlannedEvacuation" << xml::end // "Shelter|NoPlannedEvacuation|EvacuationPartial|EvacuationTotal|N/A"
         << xml::start( "EvacuationAction" ) << "Anticipated" << xml::end; // "Anticipated|InProcess|Completed|N/A"
}


// -----------------------------------------------------------------------------
// Name: HospitalUpdateReport::Send
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void HospitalUpdateReport::Send( xml::xostream& xos ) const
{
    xml::xistringstream xis( xos_->str() );
    xos << xis;
}
