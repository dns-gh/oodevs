// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_DataBase.h"
#include "Tools/MIL_Config.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

namespace
{
    void ReadRepository( xml::xistream& xis, const std::string& path, masalife::brain::library::Library::T_Paths& list )
    {
        std::string repository;
        xis >> repository;
        list.push_back( MIL_Config::BuildChildPath( path, repository ) );
    }

    masalife::brain::library::Library::T_Paths GetLibPaths( xml::xistream& xis, const std::string& strPath )
    {
        masalife::brain::library::Library::T_Paths repositories;
        xis >> xml::start( "RepertoiresBM" )
                >> xml::list( "Repertoire", boost::bind( &ReadRepository, _1, boost::ref( strPath ), boost::ref( repositories ) ) )
            >> xml::end;
        return repositories;
    }

    void ReadKnowledge( xml::xistream& xis, std::vector< const std::string >& knowledges )
    {
        std::string knowledge;
        xis >> knowledge;
        knowledges.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_DataBase constructor
// Created: MGD 2009-08-06
// -----------------------------------------------------------------------------
DEC_DataBase::DEC_DataBase( xml::xistream& xis, const std::string& strPath )
//    : masalife::brain::library::Library( GetLibPaths( xis, strPath ), std::vector< std::string >() )
{
    //@TODO MGD Change to aggregation for masalife::brain::library::Library
    libPaths_ = GetLibPaths( xis, strPath );
    xis >> xml::start( "Knowledges" )
            >> xml::list( "Knowledge", boost::bind( &ReadKnowledge, _1, boost::ref( knowledges_ ) ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_DataBase constructor
// Created: MGD 2009-08-27
// -----------------------------------------------------------------------------
DEC_DataBase::DEC_DataBase( masalife::brain::library::Library::T_Paths libPaths, const std::vector< const std::string >& knowledge )
//: masalife::brain::library::Library( libPaths, std::vector< std::string >() )
    : libPaths_  ( libPaths )
    , knowledges_( knowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_DataBase destructor
// Created: MGD 2009-08-06
// -----------------------------------------------------------------------------
DEC_DataBase::~DEC_DataBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_DataBase::InitKnowledges
// Created: MGD 2009-08-06
// -----------------------------------------------------------------------------
void DEC_DataBase::InitKnowledges( directia::brain::Brain& brain )
{
    for( std::vector< const std::string >::const_iterator it = knowledges_.begin(); it!= knowledges_.end(); it++ )
	{
        // RegisterKnowledge( brain, *it );
    }
}
