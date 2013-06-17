// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace
{
    struct OrderParameterValueVisitor : public kernel::OrderParameterValueVisitor_ABC
    {
        OrderParameterValueVisitor( actions::gui::ParamComboBox< int >& param ) : param_( &param ) {}
        virtual void Visit( const kernel::OrderParameterValue& value )
        {
            param_->AddItem( value.GetName().c_str(), value.GetId() );
        }
        actions::gui::ParamComboBox< int >* param_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ParamComboBox< T >::ParamComboBox( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , comboBox_( 0 )
{
    OrderParameterValueVisitor visitor( *this );
    parameter.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ParamComboBox< T >::~ParamComboBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< typename T >
QWidget* ParamComboBox< T >::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );

    comboBox_ = new ::gui::ValuedComboBox< T >( "ParamComboBox", parent );
    comboBox_->SetSorting( true );
    for( T_Values::const_iterator it = values_.begin(); it != values_.end(); ++it )
        comboBox_->AddItem( it->first, it->second );
    layout->addWidget( comboBox_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::AddItem
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox< T >::AddItem( const QString& name, T value )
{
    values_.push_back( std::make_pair( name, value ) );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox< T >::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() )
        action.AddParameter( *new actions::parameters::Enumeration( parameter_, GetValue() ) );
    else
        action.AddParameter( *new actions::parameters::Enumeration( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::GetValue
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename T >
T ParamComboBox< T >::GetValue() const
{
    return ( comboBox_ && comboBox_->count() ) ? comboBox_->GetValue() : 0;
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::Clear
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox< T >::Clear()
{
    if( comboBox_ )
        comboBox_->clear();
    values_.clear();
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::InternalVisit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox< T >::InternalVisit( const actions::parameters::Parameter< std::string >& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
    {
        int index = comboBox_->findText( QString::fromStdString( param.GetValue() ) );
        if( index != -1 )
            comboBox_->setCurrentIndex( index );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox< T >::Visit( const actions::parameters::Enumeration& param )
{
    InternalVisit( param );
}
