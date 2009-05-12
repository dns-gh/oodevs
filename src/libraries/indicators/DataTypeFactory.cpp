// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "DataTypeFactory.h"
#include "CollectionDataType.h"
#include "ElementTypeResolver.h"
#include "SimpleDataType.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: DataTypeFactory constructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
DataTypeFactory::DataTypeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DataTypeFactory destructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
DataTypeFactory::~DataTypeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DataType_ABC > DataTypeFactory::Instanciate
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
boost::shared_ptr< DataType_ABC > DataTypeFactory::Instanciate( const std::string& type ) const
{
    return Instanciate( type, boost::shared_ptr< ElementTypeResolver >( new ElementTypeResolver() ) );
}

namespace
{
    bool IsCollection( const std::string& type )
    {
        return boost::starts_with( type, "list(" );
    }
}

// -----------------------------------------------------------------------------
// Name: DataTypeFactory::Instanciate
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DataType_ABC > DataTypeFactory::Instanciate( const std::string& type, boost::shared_ptr< ElementTypeResolver > resolver ) const
{
    if( IsCollection( type ) )
        return boost::shared_ptr< DataType_ABC >( new CollectionDataType( type, resolver ) );
    return boost::shared_ptr< DataType_ABC >( new SimpleDataType( type, resolver ) );
}
