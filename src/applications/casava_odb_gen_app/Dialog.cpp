// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Dialog.h"
#include "moc_Dialog.cpp"
#include <tools/Path.h>
#include <tools/FileWrapper.h>
#include "clients_gui/FileDialog.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>
#include <geocoord/UTM.h>
#include <geocoord/MGRS.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string/regex.hpp>

// -----------------------------------------------------------------------------
// Name: Dialog constructor
// Created: JSR 2014-05-20
// -----------------------------------------------------------------------------
Dialog::Dialog()
{
    QVBoxLayout* vLayout = new QVBoxLayout( this );
    odb_ = new QLineEdit ( "E:/models/trunk/data/exercises/tests/Casava_Test/orbat.xml" );
    data_ = new QLineEdit( "C:/Users/Joel/Downloads/LargeEvacResults_MASA/PDC_HumanImpact_scen_85%evac.txt" );
    unitType_ = new QLineEdit( "CASAVA Stroller" ) ;
    unitName_ = new QLineEdit( "Promeneur" ) ;
    automatType_ = new QLineEdit( "CASAVA Promeneurs" );
    automatName_ = new QLineEdit( "Promeneurs" ) ;

    {
        QHBoxLayout* l = new QHBoxLayout;
        QPushButton* b = new QPushButton( "Browse" );
        connect( b, SIGNAL( clicked() ), SLOT( OnBrowseODB() ) );
        l->addWidget( new QLabel( "Orbat" ) );
        l->addWidget( odb_ );
        l->addWidget( b );
        vLayout->addLayout( l );
    }

    {
        QHBoxLayout* l = new QHBoxLayout;
        QPushButton* b = new QPushButton( "Browse" );
        connect( b, SIGNAL( clicked() ), SLOT( OnBrowseData() ) );
        l->addWidget( new QLabel( "Human impact" ) );
        l->addWidget( data_ );
        l->addWidget( b );
        vLayout->addLayout( l );
    }

    {
        QHBoxLayout* l = new QHBoxLayout;
        l->addWidget( new QLabel( "Unit type" ) );
        l->addWidget( unitType_ );
        vLayout->addLayout( l );
    }

    {
        QHBoxLayout* l = new QHBoxLayout;
        l->addWidget( new QLabel( "Unit name" ) );
        l->addWidget( unitName_ );
        vLayout->addLayout( l );
    }

    {
        QHBoxLayout* l = new QHBoxLayout;
        l->addWidget( new QLabel( "Automat type" ) );
        l->addWidget( automatType_ );
        vLayout->addLayout( l );
    }

    {
        QHBoxLayout* l = new QHBoxLayout;
        l->addWidget( new QLabel( "Automat name" ) );
        l->addWidget( automatName_ );
        vLayout->addLayout( l );
    }


    QPushButton* b = new QPushButton( "Generate" );
    connect( b, SIGNAL( clicked() ), SLOT( OnGenerate() ) );

    vLayout->addWidget( b );
}

// -----------------------------------------------------------------------------
// Name: Dialog destructor
// Created: JSR 2014-05-20
// -----------------------------------------------------------------------------
Dialog::~Dialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dialog::OnBrowseODB
// Created: JSR 2014-05-20
// -----------------------------------------------------------------------------
void Dialog::OnBrowseODB()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, "ODB" );
    if( !filename.IsEmpty() )
        odb_->setText( filename.ToUTF8().c_str() );
}

// -----------------------------------------------------------------------------
// Name: Dialog::OnBrowseData
// Created: JSR 2014-05-20
// -----------------------------------------------------------------------------
void Dialog::OnBrowseData()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, "data" );
    if( !filename.IsEmpty() )
        data_->setText( filename.ToUTF8().c_str() );
}

#define STR( _id ) boost::lexical_cast< std::string >( _id )

namespace
{
    struct T_Info
    {
        std::string mgrs_;
        unsigned int TS3_;
        unsigned int TS2_;
        unsigned int TS1_;
    };

    typedef std::vector< T_Info > T_Infos;

    unsigned long FindMaxIdInFile( const tools::Path& filePath )
    {
        static const boost::regex idRegex( "id=\"([0-9]+)\"" );
        unsigned long maxId = 0;
        std::string line;
        tools::Ifstream ifile( filePath );
        while( std::getline( ifile, line ) )
        {
            boost::sregex_iterator it( line.begin(), line.end(), idRegex );
            const boost::sregex_iterator end;
            for( ; it != end; ++it )
                for( int i = 1; i < static_cast< int >( it->size() ); ++i )
                    maxId = std::max( maxId,
                    boost::lexical_cast< unsigned long >( it->str( i ) ));
        }
        return maxId;
    }

    unsigned long FindMaxId( const tools::Path& orbatPath )
    {
        unsigned long maxOrbatId = FindMaxIdInFile( orbatPath );
        tools::Path urbanPath = orbatPath.Parent() / "urban.xml";
        unsigned long maxUrbanId = urbanPath.Exists()? FindMaxIdInFile( urbanPath ) : 0;
        return std::max( maxUrbanId, maxOrbatId ) + 2;
    }
}

// -----------------------------------------------------------------------------
// Name: Dialog::GetUnitString
// Created: JSR 2014-05-20
// -----------------------------------------------------------------------------
std::string Dialog::GetUnitString( unsigned int id, const std::string& mgrs, const std::string& wound, bool& pc ) const
{
    std::string ret = 
        "<unit command-post=\"" + ( pc ? std::string( "true" ) : std::string( "false" ) ) + "\" id=\"" + STR( id ) +
        "\" name=\"" + unitName_->text().toStdString() + "\" position=\"" + mgrs +
        "\" type=\"" + unitType_->text().toStdString() + "\">"
        "<humans>"
        "<human number=\"1\" rank=\"MilitaireDuRang\" state=\"" + wound + "\"/>"
        "</humans>"
        "</unit>";
    pc = false;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: Dialog::OnGenerate
// Created: JSR 2014-05-20
// -----------------------------------------------------------------------------
void Dialog::OnGenerate()
{
    const QString odb = odb_->text();
    const QString data = data_->text();

    T_Infos infos;

    tools::Ifstream stream( tools::Path::FromUTF8( data.toStdString() ) );
    std::string line;
    std::getline( stream, line ); // entête
    while( std::getline( stream, line ) )
    {
        boost::algorithm::trim( line );
        std::vector< std::string > inputs;
        boost::algorithm::split( inputs, line, boost::algorithm::is_any_of( " \t" ), boost::algorithm::token_compress_on );
        const double xUTM = boost::lexical_cast< double >( inputs[ 1 ] );
        const double yUTM = boost::lexical_cast< double >( inputs[ 2 ] );
        //unsigned int dead = boost::lexical_cast< unsigned int >( inputs[ 3 ] );
        unsigned int TS3 = boost::lexical_cast< unsigned int >( inputs[ 4 ] );
        unsigned int TS2 = boost::lexical_cast< unsigned int >( inputs[ 5 ] );
        unsigned int TS1 = boost::lexical_cast< unsigned int >( inputs[ 6 ] );
        //unsigned int healthy  = boost::lexical_cast< unsigned int >( inputs[ 7 ] );

        if( TS3 || TS2 || TS1 )
        {
            geocoord::UTM utm( geocoord::eNorth, 20, yUTM, xUTM );
            geocoord::MGRS mgrs( utm );
            T_Info info;
            info.mgrs_ = mgrs.GetString();
            info.TS3_ = TS3;
            info.TS2_ = TS2;
            info.TS1_ = TS1;
            infos.push_back( info );
        }
    }

    tools::Path odbPath = tools::Path::FromUTF8( odb.toStdString() );
    unsigned long id = FindMaxId( odbPath );
    unsigned long idKg = id++;

    std::string tmp =  
        "<party id=\"" + STR( id++) + "\" name=\"Promeneurs\" type=\"friend\">";
    tmp += "<objects/>"
          "<tactical>"
            "<formation id=\"" + STR( id++ ) + "\" level=\"xx\" name=\"Formation\" symbol=\"symbols/sfgpue2\">";
    tmp += "<automat engaged=\"false\" id=\"" + STR( id++ ) + "\" knowledge-group=\"" + STR( idKg ) +
        "\" name=\"" + automatName_->text().toStdString() + "\" symbol=\"symbols/sfgpue2\""
        + " type=\"" + automatType_->text().toStdString() + "\">";
    bool pc = true;
    for( auto it = infos.begin(); it != infos.end(); ++it )
    {
        for( unsigned int i = 0; i < it->TS1_; ++i )
            tmp += GetUnitString( id++, it->mgrs_, "u1", pc );
        for( unsigned int i = 0; i < it->TS2_; ++i )
            tmp += GetUnitString( id++, it->mgrs_, "u2", pc );
        for( unsigned int i = 0; i < it->TS3_; ++i )
            tmp += GetUnitString( id++, it->mgrs_, "u3", pc );
    }
    tmp += "</automat>"
        "</formation>"
          "</tactical>"
          "<communication>"
            "<knowledge-group id=\"" + STR( idKg ) + "\" name=\"Groupe de connaissance\" type=\"Standard\"/>"
          "</communication>"
          "<populations/>"
          "<inhabitants/>"
          "<logistics/>"
        "</party>";


    std::string result;
    tools::Ifstream stream2( odbPath );
    bool merged = false;
    while( std::getline( stream2, line ) )
    {
        if( !merged && ( line.find( "/parties" ) != line.npos  || line.find( "<no-party>" ) != line.npos ) )
        {
            result += tmp;
            merged = true;
        }
        result += line;
    }
    xml::xistringstream tmpxis( result );
    tools::Xofstream xos( odbPath.Parent() / "orbat_modified.xml" );
    xos << tmpxis;
}
