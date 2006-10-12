// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LimitsModel.h"
#include "TacticalLine_ABC.h"
#include "Limit.h"
#include "Lima.h"
#include "clients_kernel/Controllers.h"
#include "Publisher_ABC.h"
#include "xeumeuleu/xml.h"

// -----------------------------------------------------------------------------
// Name: LimitsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::LimitsModel( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, Publisher_ABC& publisher )
    : controllers_( controllers )
    , converter_( converter )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::~LimitsModel()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::UpdateToSim
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::UpdateToSim()
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        TacticalLine_ABC& line = *it->second;
        line.UpdateToSim();
    }
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::UseSimTacticalLines
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::UseSimTacticalLines()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const ASN1T_MsgLimitCreation& asnMsg )
{
    TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, asnMsg, converter_ );
    Register( asnMsg.oid, *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const ASN1T_MsgLimaCreation& asnMsg )
{
    TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, asnMsg, converter_ );
    Register( asnMsg.oid, *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLimit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsModel::CreateLimit( const T_PointVector& points )
{
    TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, points, converter_ );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLima
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsModel::CreateLima( E_FuncLimaType type, const T_PointVector& points )
{
    TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, points, type, converter_ );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLimit
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLimit( unsigned long id )
{
    TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLima
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLima( unsigned long id )
{
    TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Load
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void LimitsModel::Load( const std::string& tacticalLines )
{
    xml::xifstream xis( tacticalLines );
    xis >> xml::start( "lines" )
            >> xml::list( *this, &LimitsModel::ReadLine )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::ReadLine
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void LimitsModel::ReadLine( const std::string& name, xml::xistream& xis )
{
    TacticalLine_ABC* line = 0;
    if( name == "lima" )
        line = new Lima( controllers_.controller_, publisher_, xis, converter_ );
    else if( name == "limit" )
        line = new Limit( controllers_.controller_, publisher_, xis, converter_ );
    if( line )
        Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Save
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void LimitsModel::Save( const std::string& tacticalLines ) const
{
    xml::xofstream xos( tacticalLines );
    xos << xml::start( "lines" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        TacticalLine_ABC& line = *it->second;
        line.Serialize( xos );
    }
    xos << xml::end();
}
