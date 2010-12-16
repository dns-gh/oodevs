// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "UpdateListener.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Controller.h"
#include "dispatcher/Model.h"
#include "dispatcher/Object_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/NullClientPublisher.h"
#include "protocol/SimulationSenders.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "actions/ObjectMagicAction.h"
#include <xeumeuleu/xml.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace plugins::edxl;

// -----------------------------------------------------------------------------
// Name: UpdateListener constructor
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
UpdateListener::UpdateListener( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& publisher )
    : model_ ( model )
    , static_ ( staticModel )
    , publisher_ ( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UpdateListener destructor
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
UpdateListener::~UpdateListener()
{
    // NOTHING
}

namespace
{
    template< typename T >
    class manipulator
    {
    public:
        explicit manipulator( T functor ) : functor_ ( functor ) {}

        friend xml::xistream& operator >>( xml::xistream& xis, const manipulator< T >& m )
        {
            m.functor_( xis );
            return xis;
        }
    private:
        T functor_;
    };

    template< typename T >
    manipulator< T > xml_bind( T value )
    {
        return manipulator< T >( value );
    }
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::Handle
/*
    <EDXLDistribution xmlns="urn:oasis:names:tc:emergency:EDXL:DE:1.0"/>
        <HospitalStatus xmlns="urn:HAVBED:WEBSERVICE:DATA:REPORTING:HAVE:VER:2"/>
*/
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void UpdateListener::Handle( xml::xisubstream xis )
{
    // skip encapsulated information EDXL-ED
    xis >> xml::start( "EDXLDistribution" )
            >> xml::start( "contentObject" )
                >> xml::start( "xmlContent" )
                    >> xml::start( "embeddedXMLContent" )
                        >> xml_bind( boost::bind( &UpdateListener::ReadHospitalStatus, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::ReadHospitalStatus
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
void UpdateListener::ReadHospitalStatus( xml::xistream& xis )
{
    xis >> xml::start( "HospitalStatus" )
            >> xml::list( "Hospital", *this, &UpdateListener::SendHospital );
}

namespace
{
    class ReferenceValidator : public dispatcher::NullClientPublisher, private boost::noncopyable
    {
    public:
        explicit ReferenceValidator( const std::string& id, const dispatcher::Object_ABC& checked ) 
            : id_ ( id ), checked_( checked ), match_ ( false ) {}

        virtual void Send( const sword::SimToClient& asn ) 
        {
            if ( asn.has_message() && asn.message().has_object_update() )
            {
                const sword::ObjectAttributes& attributes = asn.message().object_update().attributes();
                match_ = attributes.has_medical_treatment() && attributes.medical_treatment().external_reference_id() == id_;
            }
        }

        bool IsOK() const
        {
            return match_;
        }

    private:
        const std::string& id_;
        const dispatcher::Object_ABC& checked_;
        bool match_;
    };

    const dispatcher::Object_ABC* FindHospital( const tools::Resolver_ABC< dispatcher::Object_ABC >& objects, const std::string& id )
    {
        tools::Iterator< const dispatcher::Object_ABC& > it = objects.CreateIterator();
        while ( it.HasMoreElements() )
        {
            const dispatcher::Object_ABC& object = it.NextElement();
            if( object.GetType().HasMedicalCapacity() )
            {
                ReferenceValidator validator( id, object );
                
                object.SendFullUpdate( validator );
                if( validator.IsOK() )
                    return &object;
            }
        }
        return static_cast< const dispatcher::Object_ABC* >( 0 );
    }

    const kernel::OrderParameter& Retrieve( const kernel::MagicActionType& actionType, const std::string& name )
    {
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        while ( it.HasMoreElements() )
        {
            const kernel::OrderParameter& order = it.NextElement();
            if ( order.GetName() == name )
                return order;
        }
        throw std::runtime_error( "unknown parameter : " + name );
    }
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::SendHospital
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void UpdateListener::SendHospital( xml::xistream& xis )
{
    std::string id;
    xis >> xml::start( "OrganizationInformation" )
            >> xml::content( "OrganizationID", id )
        >> xml::end;
    const dispatcher::Object_ABC* hospital = FindHospital( model_.Objects(), id );
    if( hospital != 0 )
    {
        kernel::Controller controller;
        simulation::ObjectMagicAction magic;
        magic().mutable_object()->set_id( hospital->GetId() );
        magic().set_type( sword::ObjectMagicAction::update );
        magic().mutable_parameters()->add_elem()->mutable_value()->Add()->set_identifier( sword::ObjectMagicAction::medical_treatment );
        sword::MissionParameter_Value* parameters = magic().mutable_parameters()->add_elem()->mutable_value()->Add()->add_list();
        xis >> xml::optional >> xml::start( "HospitalBedCapacityStatus" )
                >> xml_bind( boost::bind( &UpdateListener::UpdateCapacityStatus, this, _1, boost::ref( *parameters ) ) )
            >> xml::end
            >> xml::optional >> xml::start( "HospitalFacilityStatus" )
                >> xml_bind( boost::bind( &UpdateListener::UpdateFacilityStatus, this, _1, boost::ref( *parameters ) ) )
            >> xml::end;
        magic.Send( publisher_ );
    }
}

namespace
{
    sword::ObjectAttributeMedicalTreatment_EnumMedicalTreatmentStatus TranslateStatus( const std::string& status )
    {
        if( status == "Normal" )
            return sword::ObjectAttributeMedicalTreatment::normal;
        if( status == "Advisory" )
            return sword::ObjectAttributeMedicalTreatment::normal;
        if( status == "On Divert" )
            return sword::ObjectAttributeMedicalTreatment::on_divert;
        if( status == "Closed" )
            return sword::ObjectAttributeMedicalTreatment::closed;
        throw std::runtime_error( "Unknown hospital status " + status );
    }
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::UpdateFacilityStatus
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
void UpdateListener::UpdateFacilityStatus( xml::xistream& xis, sword::MissionParameter_Value& parameters )
{
    std::string content( xis.content< std::string >( "EmergencyDepartmentStatus" ) );

    if( !content.empty() )
    {
        // parameters.AddString( "ExternalReferenceId", "" ); // Not used here !!
        // parameters.AddQuantity( "Doctors", 0 ); // Not used here !!
        parameters.add_list();
        parameters.add_list();
        parameters.add_list()->set_enumeration( TranslateStatus( content ) );
    }
    // IncreasingPatientCare
}

namespace
{
    struct Capacity
    {
        Capacity() : baseline_ ( 0 ), available_ ( -1 ), emergency_ ( -1 ) {}
        unsigned long typeId_;
        int available_;
        int baseline_;
        int emergency_;
    };

    template< typename T >
    class content_check_manipulator : boost::noncopyable
    {
    public:
        explicit content_check_manipulator( const std::string& tag, T& content )
            : tag_ ( tag ), content_ ( content )
        {}

        friend xml::xistream& operator >>( xml::xistream& xis, content_check_manipulator< T > m )
        {
            xis >> xml::start( m.tag_ );
            if( xis.has_content() )
                xis >> m.content_;
            xis >> xml::end;
            return xis;
        }

    private:
        const std::string& tag_;
        T& content_;
    };

    template< typename T >
    content_check_manipulator< T > content_check( const std::string& tag, T& content )
    {
        return content_check_manipulator< T >( tag, content );
    }

    struct CapacityReader
    {
        void Read( xml::xistream& xis, std::vector< Capacity >& container, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver )
        {
            unsigned emergency72hr;
            std::string status, type;

            xis >> xml::start( "BedType" ) >> type >> xml::end;
            if( const kernel::MedicalTreatmentType* treatment = resolver.Find( type ) )
            {
                container.resize( container.size() + 1 );
                Capacity& capacity = container.back();

                capacity.typeId_ = treatment->GetId();
                xis >> xml::start( "Capacity" )
                        >> xml::content( "CapacityStatus", status )
                        >> xml::optional >> content_check( "BaselineCount", capacity.baseline_ )
                        >> xml::optional >> content_check( "AvailableCount", capacity.available_ )
                        >> xml::optional >> content_check( "AdditionalCapacityCount24Hr", capacity.emergency_ )
                        >> xml::optional >> content_check( "AdditionalCapacityCount72Hr", emergency72hr )
                    >> xml::end;
            }
        }
    };

    void FillParameters( const std::vector< Capacity >& container, sword::MissionParameter_Value& parameters )
    {
        typedef std::vector< Capacity >::const_iterator const_iterator;

        int i = 0;
        for ( const_iterator it = container.begin(); it < container.end(); ++it, ++i )
        {
            sword::MissionParameter_Value& bed = *parameters.add_list(); // "BedCapacity"
            sword::MissionParameter_Value* param;

            bed.add_list()->set_identifier( it->typeId_ );
            param = bed.add_list();
            if( it->baseline_ >= 0 )
                param->set_quantity( it->baseline_ );
            // bed.AddQuantity( "BaseLineCount", it->baseline_ );
            int available = it->available_;
            if( available < 0 )
                available = it->baseline_;
            bed.add_list()->set_quantity( available );
            // bed.AddQuantity( "AvailableCount",  available );
            param = bed.add_list();
            if( it->emergency_ >= 0 )
                param->set_quantity( it->emergency_ );
                // bed.AddQuantity( "EmergencyCount", it->emergency_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::UpdateCapacityStatus
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
void UpdateListener::UpdateCapacityStatus( xml::xistream& xis, sword::MissionParameter_Value& parameters )
{
    // MedicalTreatmentCapacity
    std::vector< Capacity > container;
    const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver = static_.objectTypes_;
    CapacityReader reader;
    xis >> xml::list( "BedCapacity", reader, &CapacityReader::Read, boost::ref( container ), boost::cref( resolver ) );
    FillParameters( container, *parameters.add_list() ); // BedCapacities
}
