// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "tools/SessionConfig.h"
#include "tools/RealFileLoaderObserver_ABC.h"
#include "tools/Resolver.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>

namespace
{
    struct VoidRealFileLoaderObserver : tools::RealFileLoaderObserver_ABC
    {
        virtual bool NotifyInvalidXml          ( const std::string& , const xml::exception&  ) { return true; }
        virtual void NotifyNoXmlSchemaSpecified( const std::string& ) {}
        virtual void NotifyFileMigrated        ( const std::string&  , const std::string& , const std::string& ) {}
    };

    char* CMD_LINE[] = { "blah.exe",
                        "--root-dir=../../data",
                        "--exercise=worldwide/Egypt",
                        "--session=default" };

    class Fixture
    {
    public:
        Fixture()
            : config_( fileObserver_ )
        {
            config_.Parse( sizeof(CMD_LINE)/sizeof(CMD_LINE[0]), CMD_LINE );
            pluginRoot_ = config_.BuildPluginDirectory( "hla" );
            xml::xifstream xis(pluginRoot_+"/configuration.xml");
            xis >> xml::start("configuration")
                    >> xml::start("mappings")
                        >> xml::content("aggregate", aggregateMapping_)
                        >> xml::content("surface-vessel", surfaceMapping_)
                        >> xml::content("component", componentMapping_)
                        >> xml::content("munition", munitionMapping_);
            staticModel_.Load( config_ );
        }
        ~Fixture()
        {
            staticModel_.Purge();
        }

        const std::string& PluginRoot() const { return pluginRoot_; }
        const tools::ExerciseConfig& Config() const { return config_; }
        std::string AggregateMappingFile() const { return pluginRoot_+"/"+ aggregateMapping_; }
        std::string SurfaceMappingFile() const { return pluginRoot_+"/"+ surfaceMapping_; }
        std::string ComponentMappingFile() const { return pluginRoot_+"/"+ componentMapping_; }
        std::string MunitionMappingFile() const { return pluginRoot_+"/"+ munitionMapping_; }
        const kernel::StaticModel& StaticModel() const { return staticModel_; }

    private:
        VoidRealFileLoaderObserver fileObserver_;
        tools::SessionConfig config_;
        std::string pluginRoot_;
        std::string aggregateMapping_, surfaceMapping_, componentMapping_, munitionMapping_;
        kernel::StaticModel staticModel_;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_read_config, Fixture )
{
    BOOST_CHECK( PluginRoot().size() != 0 );
    BOOST_CHECK( AggregateMappingFile().size() > (PluginRoot().size()+1) );
    BOOST_CHECK( SurfaceMappingFile().size() > (PluginRoot().size()+1) );
    BOOST_CHECK( ComponentMappingFile().size() > (PluginRoot().size()+1) );
    BOOST_CHECK( MunitionMappingFile().size() > (PluginRoot().size()+1) );
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
    void parseMapping(const std::string& file, const F& ftor)
    {
        xml::xifstream xis(file);
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
        CheckEntry(const tools::Resolver_ABC<T, std::string>& resolver, const std::string& file)
            : resolver_( resolver )
            , file_( file )
        {}
        void operator()(const std::string& name) const
        {
            T* obj = resolver_.Find( name ) ;
            BOOST_CHECK_MESSAGE( obj != 0, "Unknown model "+name+ " in " + file_  );
        }
    private:
        CheckEntry(const CheckEntry<T>&);
        const CheckEntry<T>& operator=(const CheckEntry<T>&);
        const tools::Resolver_ABC<T, std::string>& resolver_;
        const std::string file_;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_check_aggreagate_mapping, Fixture )
{
    const kernel::StaticModel& st( StaticModel() );
    CheckEntry< kernel::AgentType > checker( st.types_ , AggregateMappingFile());
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
