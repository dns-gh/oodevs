// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ParamStringEnumeration.h"
#include "moc_ParamStringEnumeration.cpp"
#include "StringQVButtonGroup.h"
#include "actions/Parameter.h"
#include "actions/ParameterContainer_ABC.h"
#include <boost/bind.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration constructor
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
ParamStringEnumeration::ParamStringEnumeration( QObject* parent, const actions::gui::ParamInterface_ABC& paramInterface, const QString& title, const kernel::OrderParameter& parameter, const std::map< std::string, std::string >& values )
    : actions::gui::Param_ABC( parent, paramInterface, parameter )
    , title_( title )
    , values_( values )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration destructor
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
ParamStringEnumeration::~ParamStringEnumeration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::BuildInterface
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
QWidget* ParamStringEnumeration::BuildInterface( QWidget* parent )
{
    StringQVButtonGroup* group = new StringQVButtonGroup( title_, parent );
    std::for_each( values_.begin(), values_.end(), boost::bind( &ParamStringEnumeration::AddItem, this, boost::ref( group ), _1 ) );
    connect( group, SIGNAL( clicked( const std::string& ) ), SLOT( OnToggle( const std::string& ) ) );
    return group;
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::OnToggle
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
void ParamStringEnumeration::OnToggle( const std::string& id )
{
    bool& selected = selected_[ id ];
    selected = !selected;
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::CheckValidity
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
bool ParamStringEnumeration::CheckValidity()
{
    return true;
}

namespace
{
    struct StringParameter : public actions::parameters::Parameter< QString >
    {
    public:
        StringParameter( const kernel::OrderParameter& parameter, const QString& value )
            : actions::parameters::Parameter< QString >( parameter, value )
        {}
        virtual ~StringParameter() {}
        virtual void CommitTo( std::string& content ) const
        {
            content += GetValue().ascii();
        }
        virtual std::string SerializeType() const
        {
            return "string";
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::CommitTo
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
void ParamStringEnumeration::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    action.AddParameter( *new StringParameter( parameter_, GetValue() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::AddItem
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
void ParamStringEnumeration::AddItem( StringQVButtonGroup* group, const std::pair<std::string, std::string>& name )
{
    group->InsertCheckbox( name.first );
    selected_[ name.first ] = false;
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::GetValue
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
QString ParamStringEnumeration::GetValue() const
{
    std::vector< std::string > result;
    BOOST_FOREACH( const T_Items::value_type& val, selected_ )
    {
        CIT_Values it = values_.find( val.first );
        if( it != values_.end() && val.second )
            result.push_back( it->second );
    }
    return result.empty() ? "" : boost::join( result, "," ).c_str();
}
