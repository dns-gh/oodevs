// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ClassBuilders_h
#define plugins_hla_ClassBuilders_h

#include "ClassBuilder_ABC.h"
#include "Federate_ABC.h"
#include "HlaObject_ABC.h"
#include "Federate_ABC.h"
#include <hla/Class.h>
#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>
#include <hla/FederateIdentifier.h>
#include <boost/assign.hpp>
#include <algorithm>

namespace plugins
{
namespace hla
{
    class EmptyFederate : public Federate_ABC
    {
        virtual bool Connect() { return true; }
        virtual void Disconnect() {}
        virtual bool Create( const std::string&, const std::string& ) { return true; }
        virtual bool Create( const std::string&, const T_FomFiles& ) { return true; }
        virtual bool Destroy( const std::string& ) { return true; }
        virtual bool Join( const std::string&, bool, bool ) { return true; }
        virtual void Resign() {}
        virtual void Step() {}
        virtual void Tick() {}
        virtual void Register( const ::hla::ClassIdentifier&, ::hla::Class_ABC&, bool, bool ) {}
        virtual void Register( const ::hla::InteractionIdentifier&, ::hla::Interaction_ABC&, bool, bool ) {}
        virtual void Register( ::hla::FederateAmbassador_ABC& ) {}
        virtual void DivestRequest( const ::hla::ObjectIdentifier& , const T_AttributeIdentifiers&, const ::hla::VariableLengthData& ) {}
        virtual void UnconditionalDivest( const ::hla::ObjectIdentifier& , const T_AttributeIdentifiers& ) {}
        virtual void AcquisitionRequest( const ::hla::ObjectIdentifier& , const T_AttributeIdentifiers& ) {}
        virtual void UnconditionalAcquisition( const ::hla::ObjectIdentifier&, const T_AttributeIdentifiers&, const ::hla::VariableLengthData& ) {}
        virtual const ::hla::FederateIdentifier& GetFederateHandle() const { static const ::hla::FederateIdentifier handle; return handle; }
    };
    class ClassBuilder : public ClassBuilder_ABC
    {
    protected:
                 ClassBuilder( const std::string& name, bool publish, bool subscribe, const std::vector< std::string >& attributes )
                     : name_      ( name )
                     , publish_   ( publish )
                     , subscribe_ ( subscribe )
                     , attributes_( attributes.begin(), attributes.end() )
                 { }
        virtual ~ClassBuilder() {}

    public:
        virtual void Build( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass ) const
        {
            std::for_each( attributes_.begin(), attributes_.end(), [&](const std::string& attribute)
                {
                    hlaClass.Register( ::hla::AttributeIdentifier( attribute ) );
                });
            hlaClass.ActivateUpdates( true );
            federate.Register( ::hla::ClassIdentifier( name_ ), hlaClass, publish_, subscribe_ );
        }
        virtual void GetAttributes( std::vector< std::string>& attributes ) const
        {
            attributes.insert( attributes.end(), attributes_.begin(), attributes_.end() );
        }
    private:
        const std::string name_;
        const bool publish_;
        const bool subscribe_;
        const std::vector< std::string > attributes_;
    };
    class NetnClassBuilder : public ClassBuilder
    {
    protected:
                 NetnClassBuilder( const std::string& name, bool publish, bool subscribe, const std::vector< std::string >& attributes, std::unique_ptr< ClassBuilder_ABC > builder )
                     : ClassBuilder( name, publish, subscribe, attributes )
                     , builder_( std::move( builder ) )
                 {}
        virtual ~NetnClassBuilder() {}
    public:
        virtual void Build( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass ) const
        {
            EmptyFederate empty;
            builder_->Build( empty, hlaClass );
            ClassBuilder::Build( federate, hlaClass );
        }
        virtual void GetAttributes( std::vector< std::string>& attributes ) const
        {
            builder_->GetAttributes( attributes );
            ClassBuilder::GetAttributes( attributes );
        }
    private:
        std::unique_ptr< ClassBuilder_ABC > builder_;
    };
    namespace details
    {
        inline std::vector< std::string > BuildAttributeList( bool isHla13, const std::vector< std::string>& attributes )
        {
            std::vector< std::string > retval( 1, isHla13 ? "privilegeToDelete" : "HLAprivilegeToDeleteObject" );
            retval.insert( retval.end(), attributes.begin(), attributes.end() );
            return retval;
        }
    }

    class AggregateEntityBuilder : public ClassBuilder
    {
    public:
        AggregateEntityBuilder( bool isHla13 = false )
            : ClassBuilder( "BaseEntity.AggregateEntity", true, true, 
                    details::BuildAttributeList( isHla13, boost::assign::list_of( "EntityType" )
                                                            ( "EntityIdentifier" )
                                                            ( "ForceIdentifier" )
                                                            ( "AggregateMarking" )
                                                            ( "AggregateState" )
                                                            ( "Dimensions" )
                                                            ( "Spatial" )
                                                            ( "Formation" )
                                                            ( "NumberOfSilentEntities" )
                                                            ( "SilentEntities" )
                                                            ( "SubAggregateIdentifiers" )
                                                            ( "NumberOfVariableDatums" )
                                                            ( "RelativeSpatial" )
                                                            ( "EntityIdentifiers" )
                                                            ( "VariableDatums" ) ) )
        {}
    };
    class NetnAggregateEntityBuilder : public NetnClassBuilder
    {
    public:
        NetnAggregateEntityBuilder( bool isHla13 = false )
            : NetnClassBuilder( "BaseEntity.AggregateEntity.NETN_Aggregate", true, true
                    , boost::assign::list_of( "Mounted" )
                                    ( "Echelon" )
                                    ( "UniqueID" )
                                    ( "HigherHeadquarters" )
                                    ( "Callsign" )
                                    ( "Status" )
                                    ( "Symbol" )
                                    ( "EmbeddedUnitList" )
            , std::unique_ptr< ClassBuilder_ABC >( new AggregateEntityBuilder( isHla13 ) ) )
        {}
    };
    class SurfaceVesselBuilder : public ClassBuilder
    {
    public:
        SurfaceVesselBuilder( bool isHla13 = false )
            : ClassBuilder( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of ( "EntityType" )
                                                                  ( "EntityIdentifier" )
                                                                  ( "ForceIdentifier" )
                                                                  ( "Marking" )
                                                                  ( "Spatial" )
                                                                  ( "IsPartOf" )
                                                                  ( "DamageState" )
                                                                  ( "IsConcealed" ) ) )
        {}
    };
    class NetnSurfaceVesselBuilder : public NetnClassBuilder
    {
    public:
        NetnSurfaceVesselBuilder( bool isHla13 = false )
            : NetnClassBuilder( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel", true, true
                    , boost::assign::list_of( "UniqueID" )
                                            ( "Callsign" )
                                            ( "EmbeddedUnitList" )
            , std::unique_ptr< ClassBuilder_ABC >( new SurfaceVesselBuilder( isHla13 ) ) )
        {}
    };
    class AircraftBuilder : public ClassBuilder
    {
    public:
        AircraftBuilder( bool isHla13 = false )
            : ClassBuilder( "BaseEntity.PhysicalEntity.Platform.Aircraft", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "EntityType" )
                                                                                 ( "EntityIdentifier" )
                                                                                 ( "ForceIdentifier" )
                                                                                 ( "Marking" )
                                                                                 ( "Spatial" )
                                                                                 ( "IsPartOf" )
                                                                                 ( "DamageState" )
                                                                                 ( "IsConcealed" ) ) )
        {}
    };
    class NetnAircraftBuilder : public NetnClassBuilder
    {
    public:
        NetnAircraftBuilder( bool isHla13 = false )
            : NetnClassBuilder( "BaseEntity.PhysicalEntity.Platform.Aircraft.NETN_Aircraft", true, true
            , boost::assign::list_of( "UniqueID" )
                                    ( "Callsign" )
                                    ( "EmbeddedUnitList" )
            , std::unique_ptr< ClassBuilder_ABC >( new AircraftBuilder( isHla13 ) ) )
        {}
    };

    class GroundVehicleBuilder : public ClassBuilder
    {
    public:
        GroundVehicleBuilder( bool isHla13 = false )
            : ClassBuilder( "BaseEntity.PhysicalEntity.Platform.GroundVehicle", true, false
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "EntityType" )
                                                                                 ( "EntityIdentifier" )
                                                                                 ( "ForceIdentifier" )
                                                                                 ( "Marking" )
                                                                                 ( "Spatial" )
                                                                                 ( "IsPartOf" )
                                                                                 ( "DamageState" )
                                                                                 ( "IsConcealed" ) ) )
        {}
    };
    class NetnGroundVehicleBuilder : public NetnClassBuilder
    {
    public:
        NetnGroundVehicleBuilder( bool isHla13 = false )
            : NetnClassBuilder( "BaseEntity.PhysicalEntity.Platform.GroundVehicle.NETN_GroundVehicle", true, false
                    , boost::assign::list_of( "UniqueID" )
                                            ( "Callsign" )
                                            ( "EmbeddedUnitList" )
            , std::unique_ptr< ClassBuilder_ABC >( new GroundVehicleBuilder( isHla13 ) ) )
        {}
    };

    class HumanBuilder : public ClassBuilder
    {
    public:
        HumanBuilder( bool isHla13 = false )
            : ClassBuilder( "BaseEntity.PhysicalEntity.Lifeform.Human", true, false
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "EntityType" )
                                                                                 ( "EntityIdentifier" )
                                                                                 ( "ForceIdentifier" )
                                                                                 ( "Marking" )
                                                                                 ( "Spatial" )
                                                                                 ( "IsPartOf" )
                                                                                 ( "DamageState" )
                                                                                 ( "IsConcealed" ) ) )
        {}
    };
    class NetnHumanBuilder : public NetnClassBuilder
    {
    public:
        NetnHumanBuilder( bool isHla13 = false )
            : NetnClassBuilder( "BaseEntity.PhysicalEntity.Lifeform.Human.NETN_Human", true, false
                    , boost::assign::list_of( "UniqueID" )
                                    ( "Callsign" )
            , std::unique_ptr< ClassBuilder_ABC >( new HumanBuilder( isHla13 ) ) )
        {}
    };

    // Objects
    class CulturalFeaturedBuilder : public ClassBuilder
    {
    public:
        CulturalFeaturedBuilder( bool isHla13 = false )
            : ClassBuilder( "BaseEntity.PhysicalEntity.CulturalFeature", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "EntityType" )
                                                                                ( "EntityIdentifier" )
                                                                                ( "ForceIdentifier" )
                                                                                ( "Marking" )
                                                                                ( "Spatial" ) ) )
        {}
    };
    class MinefieldBuilder : public ClassBuilder
    {
    public:
        MinefieldBuilder( bool isHla13 = false )
            : ClassBuilder( "Minefield", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "ActiveStatus" )
                                                                                 ( "ForceIdentifier" )
                                                                                 ( "MinefieldIdentifier" )
                                                                                 ( "MinefieldLocation" )
                                                                                 ( "MinefieldOrientation" )
                                                                                 ( "MinefieldType" )
                                                                                 ( "MineTypes" )
                                                                                 ( "PerimeterPointCoordinates" )
                                                                                 ( "ProtocolMode" )
                                                                                 ( "State" ) ) )
        {}
    };
    class OtherPointObjectBuilder : public ClassBuilder
    {
    public:
        OtherPointObjectBuilder( bool isHla13 = false )
            : ClassBuilder( "EnvironmentObject.PointObject.OtherPointObject", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "ObjectIdentifier" )
                                                                                  ( "ReferencedObjectIdentifier" )
                                                                                  ( "ForceIdentifier" )
                                                                                  ( "ObjectType" )
                                                                                  ( "Flaming" )
                                                                                  ( "Smoking" )
                                                                                  ( "Deactivated" )
                                                                                  ( "ObjectPreDistributed" )
                                                                                  ( "PercentComplete" )
                                                                                  ( "DamagedAppearance" )
                                                                                  ( "Orientation" )
                                                                                  ( "Location" ) ) )
        {}
    };
    class BreachablePointObjectBuilder : public ClassBuilder
    {
    public:
        BreachablePointObjectBuilder( bool isHla13 = false )
            : ClassBuilder( "EnvironmentObject.PointObject.BreachablePointObject", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "ObjectIdentifier" )
                                                                                  ( "ReferencedObjectIdentifier" )
                                                                                  ( "ForceIdentifier" )
                                                                                  ( "ObjectType" )
                                                                                  ( "Flaming" )
                                                                                  ( "Smoking" )
                                                                                  ( "Deactivated" )
                                                                                  ( "ObjectPreDistributed" )
                                                                                  ( "PercentComplete" )
                                                                                  ( "DamagedAppearance" )
                                                                                  ( "Orientation" )
                                                                                  ( "Location" )
                                                                                  ( "BreachedStatus" ) ) )
        {}
    };
    class OtherArealObjectBuilder : public ClassBuilder
    {
    public:
        OtherArealObjectBuilder( bool isHla13 = false )
            : ClassBuilder( "EnvironmentObject.ArealObject.OtherArealObject", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "ObjectIdentifier" )
                                                                                  ( "ReferencedObjectIdentifier" )
                                                                                  ( "ForceIdentifier" )
                                                                                  ( "ObjectType" )
                                                                                  ( "Flaming" )
                                                                                  ( "Smoking" )
                                                                                  ( "Deactivated" )
                                                                                  ( "ObjectPreDistributed" )
                                                                                  ( "PercentComplete" )
                                                                                  ( "DamagedAppearance" )
                                                                                  ( "PointsData" ) ) )
        {}
    };
    class MinefieldObjectBuilder : public ClassBuilder
    {
    public:
        MinefieldObjectBuilder( bool isHla13 = false )
            : ClassBuilder( "EnvironmentObject.ArealObject.MinefieldObject", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "ObjectIdentifier" )
                                                                                  ( "ReferencedObjectIdentifier" )
                                                                                  ( "ForceIdentifier" )
                                                                                  ( "ObjectType" )
                                                                                  ( "Flaming" )
                                                                                  ( "Smoking" )
                                                                                  ( "Deactivated" )
                                                                                  ( "ObjectPreDistributed" )
                                                                                  ( "PercentComplete" )
                                                                                  ( "DamagedAppearance" )
                                                                                  ( "PointsData" )
                                                                                  ( "MineCount" )
                                                                                  ( "BreachedStatus" ) ) )
        {}
    };
    class BreachableLinearObjectBuilder : public ClassBuilder
    {
    public:
        BreachableLinearObjectBuilder( bool isHla13 = false )
            : ClassBuilder( "EnvironmentObject.LinearObject.BreachableLinearObject", true, true
                    , details::BuildAttributeList( isHla13, boost::assign::list_of( "ObjectIdentifier" )
                                                                                  ( "ReferencedObjectIdentifier" )
                                                                                  ( "ForceIdentifier" )
                                                                                  ( "ObjectType" )
                                                                                  ( "SegmentRecords" ) ) )
        {}
    };
    class RawDataHazardContourGroupBuilder : public ClassBuilder
    {
    public:
        RawDataHazardContourGroupBuilder( bool isHla13 = false )
            : ClassBuilder( "RawDataHazardContourGroup", true, true
            , details::BuildAttributeList( isHla13, boost::assign::list_of( "Time" )
                                                                          ( "Material" )
                                                                          ( "HazardType" )
                                                                          ( "Contours" ) ) )
        {}
    };
    class ATP45HazardAreaBuilder : public ClassBuilder
    {
    public:
        ATP45HazardAreaBuilder( bool isHla13 = false )
            : ClassBuilder( "ATP45HazardArea", true, true
            , details::BuildAttributeList( isHla13, boost::assign::list_of( "Locations" )
                                                                          ( "ATP45HazardAreaType" )
                                                                          ( "AgentClass" ) ) )
        {}
    };
}
}

#endif // plugins_hla_ClassBuilders_h
