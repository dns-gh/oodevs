// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamLocationCompositeList.h"
#include "ParamLocationComposite.h"
#include "actions/Action_ABC.h"
#include "actions/ParameterList.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"
#include <boost/noncopyable.hpp>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamLocationCompositeList constructor
// Created: LDC 2010-09-20
// -----------------------------------------------------------------------------
ParamLocationCompositeList::ParamLocationCompositeList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const InterfaceBuilder_ABC& builder, kernel::ActionController& controller )
    : ListParameter( parent, parameter, controller )
    , parameter_( parameter )
    , converter_( converter )
    , builder_  ( builder )
    , count_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocationCompositeList destructor
// Created: LDC 2010-09-20
// -----------------------------------------------------------------------------
ParamLocationCompositeList::~ParamLocationCompositeList()
{
    // NOTHING
}

namespace
{
    class ChoiceVisitor : private boost::noncopyable
                        , public kernel::ChoicesVisitor_ABC
    {
    public:
        ChoiceVisitor( kernel::OrderParameter& parameter )
            : parameter_( parameter )
        {}
        virtual ~ChoiceVisitor() {}
        virtual void Visit( const std::string& type )
        {
            parameter_.AddChoice( type );
        }
    private:
        kernel::OrderParameter& parameter_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamLocationCompositeList::CreateElement
// Created: LDC 2010-09-20
// -----------------------------------------------------------------------------
Param_ABC* ParamLocationCompositeList::CreateElement()
{
    kernel::OrderParameter parameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "location", false );
    ChoiceVisitor chooser( parameter );
    parameter_.Accept( chooser );
    return new ParamLocationComposite( parameter, converter_, builder_ );
}
