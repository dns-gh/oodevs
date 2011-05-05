// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "EDXLReport.h"
#include "HospitalUpdateReport.h"
#include "Publisher_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace edxl;

// -----------------------------------------------------------------------------
// Name: EDXLReport constructor
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
EDXLReport::EDXLReport( const Model& model )
    : model_ ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EDXLReport destructor
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
EDXLReport::~EDXLReport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EDXLReport::CreateReport
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
void EDXLReport::CreateReport( unsigned int id, const std::string& name, const sword::ObjectAttributeMedicalTreatment& medical )
{
    reports_[ id ].reset( new HospitalUpdateReport( name, medical, model_ ) );
}

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
// Name: EDXLReport::Publish
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
void EDXLReport::Publish( Publisher_ABC& publisher )
{
    xml::xostringstream xos;
    xos << xml::start( "EDXLDistribution" ) << xml::attribute( "xmlns", "urn:oasis:names:tc:emergency:EDXL:DE:1.0" )
            << xml::content( "distributionID", "IN.WebServiceUser" )
            << xml::content( "senderID", "support@masagroup.net" )
            << xml::content( "dateTimeSent", CurrentTime() )
            << xml::content( "distributionStatus", "Actual" )
            << xml::content( "distributionType", "Update" )
            << xml::start( "combinedConfidentiality" ) << xml::end
            << xml::start( "targetArea" ) << xml::end
            << xml::start( "contentObject" )
                << xml::content( "contentDescription", "HAVE message updating bed availabiliy for https.HAvBED.hhs.gov for the state of Indiana" )
                << xml::start( "xmlContent" )
                    << xml::start( "embeddedXMLContent" )
                        << xml::start( "HospitalStatus" ) << xml::attribute( "xmlns", "urn:HAVBED:WEBSERVICE:DATA:REPORTING:HAVE:VER:2" )
                            << xml_bind( boost::bind( &EDXLReport::PublishHospitalStatus, this, _1 ) )
                        << xml::end
                    << xml::end
                << xml::end
            << xml::end
        << xml::end;
    publisher.PushReport( xos.str() );
}

// -----------------------------------------------------------------------------
// Name: EDXLReport::PublishHospitalStatus
// Created: JCR 2010-06-10
// -----------------------------------------------------------------------------
void EDXLReport::PublishHospitalStatus( xml::xostream& xos )
{
    T_HospitalUpdateReportVector buffer;

    buffer.reserve( reports_.size() );
    for ( T_HospitalUpdateReportMap::const_iterator it = reports_.begin(); it != reports_.end(); ++it )
        buffer.push_back( it->second );

    for ( T_HospitalUpdateReportVector::const_iterator it = buffer.begin(); it != buffer.end(); ++it )
        (*it)->Send( xos );
}
