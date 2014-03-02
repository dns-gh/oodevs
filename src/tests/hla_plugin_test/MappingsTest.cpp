// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "tools/FileWrapper.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/RealFileLoaderObserver_ABC.h"
#include "tools/SessionConfig.h"
#include "tools/XmlStreamOperators.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/MissionType.h"
#include <tools/Resolver.h>
#include <xeumeuleu/xml.hpp>

namespace
{
    char* CMD_LINE[] = { "blah.exe",
                        "--root-dir=../../data",
                        "--exercise=worldwide/Egypt",
                        "--session=default" };

    class Fixture
    {
    public:
        Fixture()
            : config_( tools::CreateNullFileLoaderObserver() )
        {
            config_.Parse( sizeof(CMD_LINE)/sizeof(CMD_LINE[0]), CMD_LINE );
            pluginRoot_ = config_.BuildPluginDirectory( "hla" );
            tools::Xifstream xis( pluginRoot_ / "configuration.xml" );
            xis >> xml::start("configuration")
                    >> xml::start("mappings")
                        >> xml::content("aggregate", aggregateMapping_)
                        >> xml::content("surface-vessel", surfaceMapping_)
                        >> xml::content("component", componentMapping_)
                        >> xml::content("munition", munitionMapping_)
                        >> xml::content("object", objectMapping_);
            staticModel_.Load( config_ );
        }
        ~Fixture()
        {
            staticModel_.Purge();
        }

        const tools::Path& PluginRoot() const { return pluginRoot_; }
        const tools::ExerciseConfig& Config() const { return config_; }
        tools::Path AggregateMappingFile() const { return pluginRoot_ / aggregateMapping_; }
        tools::Path SurfaceMappingFile() const { return pluginRoot_ / surfaceMapping_; }
        tools::Path ComponentMappingFile() const { return pluginRoot_ / componentMapping_; }
        tools::Path MunitionMappingFile() const { return pluginRoot_ / munitionMapping_; }
        tools::Path ObjectMappingFile() const { return pluginRoot_ / objectMapping_; }
        const kernel::StaticModel& StaticModel() const { return staticModel_; }

    private:
        tools::SessionConfig config_;
        tools::Path pluginRoot_;
        tools::Path aggregateMapping_, surfaceMapping_, componentMapping_, munitionMapping_, objectMapping_;
        kernel::StaticModel staticModel_;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_read_config, Fixture )
{
    BOOST_CHECK( PluginRoot().ToUnicode().size() != 0 );
    BOOST_CHECK( AggregateMappingFile().ToUnicode().size() > ( PluginRoot().ToUnicode().size() + 1 ) );
    BOOST_CHECK( SurfaceMappingFile().ToUnicode().size() > ( PluginRoot().ToUnicode().size() + 1 ) );
    BOOST_CHECK( ComponentMappingFile().ToUnicode().size() > ( PluginRoot().ToUnicode().size() + 1 ) );
    BOOST_CHECK( MunitionMappingFile().ToUnicode().size() > ( PluginRoot().ToUnicode().size() + 1 ) );
    BOOST_CHECK( ObjectMappingFile().ToUnicode().size() > ( PluginRoot().ToUnicode().size() + 1 ) );
}

namespace
{
    template <typename F>
    void parseEntry(xml::xistream& xis, const F& ftor)
    {
        std::string name;
        xis >> xml::attribute("name", name);
        ftor(name);
    }

    template <typename F>
    void parseMapping(const tools::Path& file, const F& ftor)
    {
        tools::Xifstream xis( file );
        std::string defName;
        xis >> xml::start("dis-mapping")
            >> xml::attribute("default-name", defName);
        ftor(defName);
        xis >> xml::list("entry", boost::bind( &parseEntry<F>, _1, boost::cref(ftor) ) );
        xis >> xml::end;
    }

    template <typename T>
    struct CheckEntry
    {
        CheckEntry(const tools::Resolver_ABC<T, std::string>& resolver, const tools::Path& file)
            : resolver_( resolver )
            , file_( file )
        {}
        void operator()(const std::string& name) const
        {
            T* obj = resolver_.Find( name ) ;
            BOOST_CHECK_MESSAGE( obj != 0, "Unknown model " + name + " in " + file_.ToUTF8() );
        }
    private:
        CheckEntry(const CheckEntry<T>&);
        const CheckEntry<T>& operator=(const CheckEntry<T>&);
        const tools::Resolver_ABC<T, std::string>& resolver_;
        const tools::Path file_;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_check_aggreagate_mapping, Fixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::AgentType > checker( st.types_ , AggregateMappingFile() );
    parseMapping(AggregateMappingFile(), checker);
}

BOOST_FIXTURE_TEST_CASE( hla_check_surface_mapping, Fixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::AgentType > checker( st.types_, SurfaceMappingFile() );
    parseMapping(SurfaceMappingFile(), checker);
}

BOOST_FIXTURE_TEST_CASE( hla_check_component_mapping, Fixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::ComponentType > checker( st.types_, ComponentMappingFile() );
    parseMapping(ComponentMappingFile(), checker);
}

BOOST_FIXTURE_TEST_CASE( hla_check_munition_mapping, Fixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::DotationType > checker( st.objectTypes_, MunitionMappingFile() );
    parseMapping(MunitionMappingFile(), checker);
}

BOOST_FIXTURE_TEST_CASE( hla_check_object_mapping, Fixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::ObjectType > checker( st.objectTypes_, ObjectMappingFile() );
    parseMapping(ObjectMappingFile(), checker);
}

namespace
{
    class MissionFixture
    {
    public:
        MissionFixture()
            : config_( tools::CreateNullFileLoaderObserver() )
        {
            config_.Parse( sizeof(CMD_LINE)/sizeof(CMD_LINE[0]), CMD_LINE );
            pluginRoot_ = config_.BuildPluginDirectory( "hla" );
            staticModel_.Load( config_ );
        }
        ~MissionFixture()
        {
            staticModel_.Purge();
        }

        template <typename F>
        void parseMission(const std::string& , xml::xistream& xis, const F& ftor)
        {
            const std::string mission = xis.value< std::string >();
            ftor(mission);
        }
        template <typename F>
        void parseMissions(const std::string& section, const F& ftor)
        {
            tools::Xifstream xis( pluginRoot_ / "configuration.xml" );
            xis >> xml::start("configuration")
                    >> xml::start("missions")
                        >> xml::start(section)
                            >> xml::list( *this, &MissionFixture::parseMission<F>, boost::cref(ftor) );
        }
        const tools::Path& PluginRoot() const { return pluginRoot_; }
        const tools::ExerciseConfig& Config() const { return config_; }
        const kernel::StaticModel& StaticModel() const { return staticModel_; }

    private:
        tools::SessionConfig config_;
        tools::Path pluginRoot_;
        kernel::StaticModel staticModel_;
    };
    struct MissionResolver : public tools::Resolver< kernel::MissionType, std::string >
    {
    public:
        MissionResolver( const tools::Resolver_ABC< kernel::MissionType >& missions)
        {
            tools::Iterator< const kernel::MissionType& > it=missions.CreateIterator();
            while(it.HasMoreElements())
            {
                const kernel::MissionType& elem = it.NextElement();
                if( !elem.GetType() == eMissionType_Automat )
                    Register( elem.GetName(), const_cast< kernel::MissionType& >( elem ) );
            }
        }
    };
}

BOOST_FIXTURE_TEST_CASE( hla_check_frag_orders, MissionFixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::FragOrderType > checker( st.types_, PluginRoot() );
    parseMissions( "fragOrders" , checker );
}

BOOST_FIXTURE_TEST_CASE( hla_check_transport_request_missions, MissionFixture )
{
    const kernel::StaticModel& st( StaticModel() );
    MissionResolver resolver( st.types_ );
    CheckEntry< kernel::MissionType > checker( resolver, PluginRoot() );
    parseMissions( "request" , checker );
}
