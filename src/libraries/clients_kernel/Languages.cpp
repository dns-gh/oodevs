// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Languages.h"
#include "Language.h"
#include "tools/FileWrapper.h"
#include <boost/make_shared.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Languages constructor
// Created: ABR 2013-09-30
// -----------------------------------------------------------------------------
Languages::Languages( const tools::Path& xmlFile )
{
    tools::Xifstream xis( xmlFile );
    Read( xis );
}

// -----------------------------------------------------------------------------
// Name: Languages constructor
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
Languages::Languages( xml::xistream& xis )
{
    Read( xis );
}

// -----------------------------------------------------------------------------
// Name: Languages destructor
// Created: ABR 2013-09-30
// -----------------------------------------------------------------------------
Languages::~Languages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Languages::Read
// Created: ABR 2013-09-30
// -----------------------------------------------------------------------------
void Languages::Read( xml::xistream& xis )
{
    xis >> xml::start( "languages" )
            >> xml::list( "language", [&]( xml::xistream& x)
            {
                const std::string code = x.attribute( "code", "" );
                for( auto it = languages_.begin(); it != languages_.end(); ++it )
                    if( ( *it )->GetCode() == code )
                        throw MASA_EXCEPTION( "Language code already registered: " + code );
                languages_.push_back( boost::make_shared< kernel::Language >( x ) );
            })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Language >& Languages::Get
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
const boost::shared_ptr< Language >& Languages::Get( const std::string& code ) const
{
    for( auto it = languages_.begin(); it != languages_.end(); ++it )
        if( ( *it )->GetCode() == code )
            return *it;
    throw MASA_EXCEPTION( "Unsupported language: " + code );
}

// -----------------------------------------------------------------------------
// Name: Languages::GetLanguages
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
const Languages::T_Languages& Languages::GetLanguages() const
{
    return languages_;
}
