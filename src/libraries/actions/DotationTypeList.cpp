// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "DotationTypeList.h"
#include "ParameterVisitor_ABC.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: DotationTypeList constructor
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
DotationTypeList::DotationTypeList( const kernel::OrderParameter& parameter )
    : Parameter< std::string >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationTypeList destructor
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
DotationTypeList::~DotationTypeList()
{
    // NOTHING
}

namespace
{
    struct StringSerializer : public ParameterVisitor_ABC
    {
        explicit StringSerializer( std::string& content ) : content_( &content ) {}
        virtual void Visit( const DotationType& param )
        {
            if( !content_->empty() )
                *content_ += ",";
            param.CommitTo( *content_ );
        }
        std::string* content_;
    };
}

// -----------------------------------------------------------------------------
// Name: DotationTypeList::CommitTo
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void DotationTypeList::CommitTo( std::string& content ) const
{
    StringSerializer serializer( content );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: DotationTypeList::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string DotationTypeList::SerializeType() const
{
    return "resourcetype";
}
