// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_FragOrder.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Missions_Data.h"
#include "ADN_ConsistencyChecker.h"
#include <tools/Loader_ABC.h>
#include <tools/EncodingConverter.h>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <queue>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

namespace bfs = boost::filesystem;

ADN_Missions_FragOrder::ADN_Missions_FragOrder()
    : ADN_Missions_ABC()
    , isAvailableWithoutMission_( false )
{
    // NOTHING
}

ADN_Missions_FragOrder::ADN_Missions_FragOrder( unsigned int id )
    : ADN_Missions_ABC( id )
    , isAvailableWithoutMission_( false )
{
    //NOTHING
}

ADN_Missions_FragOrder::~ADN_Missions_FragOrder()
{
    // NOTHING
}

ADN_Missions_FragOrder* ADN_Missions_FragOrder::CreateCopy()
{
    ADN_Missions_FragOrder* newFragOrder = new ADN_Missions_FragOrder();
    newFragOrder->strName_ = strName_.GetData();
    newFragOrder->missionSheetPath_ = missionSheetPath_.GetData();
    newFragOrder->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ADN_Missions_Parameter* newParam = (*it)->CreateCopy();
        newFragOrder->parameters_.AddItem( newParam );
    }
    return newFragOrder;
}

void ADN_Missions_FragOrder::ReadArchive( xml::xistream& input, const std::string& missionDir )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ )
          >> xml::optional >> xml::attribute( "available-without-mission", isAvailableWithoutMission_ )
          >> xml::list( "parameter", *this, &ADN_Missions_FragOrder::ReadParameter );
    ReadMissionSheet( missionDir );
}

namespace
{
    QString BuildDiaFragOrderType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        if( list.front() == "Pion" || list.front() == "Automate" || list.front() == "Population" )
            list[0].append( "_" );
        for( int i = 1; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && list[i] == list[i].upper() )
                list[i].append( "_" );
        return QString( "Rep_OrderConduite_%1" ).arg( list.join( "" ) );
    }
}

void ADN_Missions_FragOrder::WriteArchive( xml::xostream& output )
{
    if( diaType_.GetData().empty() )
        diaType_ = BuildDiaFragOrderType( strName_.GetData().c_str() ).toStdString();

    output << xml::start( "fragorder" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "dia-type", diaType_ )
            << xml::attribute( "id", id_ )
            << xml::attribute( "available-without-mission", isAvailableWithoutMission_ );

    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output << xml::end;
}
