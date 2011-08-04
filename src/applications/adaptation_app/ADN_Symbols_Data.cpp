// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Symbols_Data.h"
#include "ADN_Project_Data.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolRule.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// ADN_Symbols_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra constructor
// Created: SLG 2011-02-17
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsInfra::SymbolsInfra( xml::xistream& input )
: strName_ ( "" )
{
    input >> xml::attribute( "name", strName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra destructor
// Created: SLG 2011-02-17
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsInfra::~SymbolsInfra()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra::GetNodeName
// Created: SLG 2011-02-17
// -----------------------------------------------------------------------------
std::string ADN_Symbols_Data::SymbolsInfra::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra::GetItemName
// Created: SLG 2011-02-17
// -----------------------------------------------------------------------------
std::string ADN_Symbols_Data::SymbolsInfra::GetItemName()
{
    return strName_.GetData();
}


// -----------------------------------------------------------------------------
// ADN_Symbols_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data constructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Symbols_Data::ADN_Symbols_Data()
    : ADN_Data_ABC()
    , factory_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data destructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Symbols_Data::~ADN_Symbols_Data()
{
    for( IT_SymbolsInfra_Vector it = infras_.begin(); it != infras_.end(); ++it )
        delete *it;
    infras_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::FilesNeeded
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::FilesNeeded( T_StringList& /*files*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::Load
// Created: ABR 2011-04-22
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::Load( const tools::Loader_ABC& fileLoader )
{
    const std::string filename = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSymbols_.GetData();
    fileLoader.LoadFile( filename, boost::bind( &ADN_Symbols_Data::ReadArchive, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::Reset
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::Reset()
{
    infras_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::ReadArchive
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::ReadArchive( xml::xistream& xis )
{
    factory_.reset( new kernel::SymbolFactory( xis ) );

    xis >> xml::start( "app6" )
            >> xml::start( "infrastructures" )
                >> xml::list( "choice", *this, &ADN_Symbols_Data::ReadInfra )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::ReadInfra
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::ReadInfra( xml::xistream& xis )
{
    infras_.AddItem( new SymbolsInfra( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::GetSymbolFactory
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
kernel::SymbolFactory& ADN_Symbols_Data::GetSymbolFactory()
{
    return *factory_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::GetSymbolsInfras
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
ADN_Symbols_Data::T_SymbolsInfra_Vector& ADN_Symbols_Data::GetSymbolsInfras()
{
    return infras_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::FindSymbolInfra
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsInfra* ADN_Symbols_Data::FindSymbolInfra( const std::string& strName ) const
{
    for( CIT_SymbolsInfra_Vector it = infras_.begin(); it != infras_.end(); ++it )
        if( **it == strName )
            return *it;
    return 0;
}
