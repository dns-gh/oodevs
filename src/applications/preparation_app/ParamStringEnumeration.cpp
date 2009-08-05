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
#include "actions/Parameter.h"
#include "actions/ParameterContainer_ABC.h"
#include <boost/bind.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration constructor
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
ParamStringEnumeration::ParamStringEnumeration( QObject* parent, const QString& title, const kernel::OrderParameter& parameter, const std::vector< std::string >& values )
    : QObject( parent )
    , actions::gui::Param_ABC( parameter.GetName().c_str() )
    , title_( title )
    , parameter_( parameter )
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
void ParamStringEnumeration::BuildInterface( QWidget* parent )
{
    QVButtonGroup* group = new QVButtonGroup( title_, parent );
    std::for_each( values_.begin(), values_.end(), boost::bind( &ParamStringEnumeration::AddItem, this, boost::ref( group ), _1 ) );
    connect( group, SIGNAL( clicked( int ) ), SLOT( OnToggle( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::OnToggle
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
void ParamStringEnumeration::OnToggle( int id )
{
    bool& selected = selected_[ values_[ id ] ];
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
void ParamStringEnumeration::AddItem( QButtonGroup* group, const std::string& name )
{
    new QCheckBox( name.c_str(), group );
    selected_[ name ] = false;
}

// -----------------------------------------------------------------------------
// Name: ParamStringEnumeration::GetValue
// Created: SBO 2009-08-05
// -----------------------------------------------------------------------------
QString ParamStringEnumeration::GetValue() const
{
    std::vector< std::string > result;
    BOOST_FOREACH( const T_Items::value_type& val, selected_ )
        if( val.second )
            result.push_back( val.first );
    return result.empty() ? "" : boost::join( result, "," ).c_str();
}
