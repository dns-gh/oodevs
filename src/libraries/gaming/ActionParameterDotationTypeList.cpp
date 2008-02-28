// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterDotationTypeList.h"
#include "ActionParameterVisitor_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationTypeList constructor
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterDotationTypeList::ActionParameterDotationTypeList( const kernel::OrderParameter& parameter )
    : ActionParameter< std::string >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationTypeList destructor
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterDotationTypeList::~ActionParameterDotationTypeList()
{
    // NOTHING
}

namespace
{
    struct StringSerializer : public ActionParameterVisitor_ABC
    {
        explicit StringSerializer( std::string& content ) : content_( &content ) {}
        virtual void Visit( const ActionParameterDotationType& param )
        {
            if( !content_->empty() )
                *content_ += ",";
            param.CommitTo( *content_ );
        }
        std::string* content_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationTypeList::CommitTo
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterDotationTypeList::CommitTo( std::string& content ) const
{
    StringSerializer serializer( content );
    Accept( serializer );
}
