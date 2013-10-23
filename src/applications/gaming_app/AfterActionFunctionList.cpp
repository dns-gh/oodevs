// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionFunctionList.h"
#include "moc_AfterActionFunctionList.cpp"
#include "actions/Parameter_ABC.h"
#include "actions_gui/ParamLocation.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/RichSpinBox.h"
#include "gaming/AfterActionFunction.h"
#include "gaming/AfterActionModel.h"
#include "gaming/AfterActionParameter.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include <boost/bind.hpp>

Q_DECLARE_METATYPE( const AfterActionFunction* )

namespace
{
    void DeleteLayout( QLayout* layout )
    {
        if( layout )
            while( QLayoutItem* item = layout->takeAt( 0 ) )
            {
                delete item->widget();
                DeleteLayout( item->layout() );
                delete item;
            }
            delete layout;
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList constructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, AfterActionModel& model, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : QWidget( parent )
    , builder_    ( interfaceBuilder )
    , controllers_( controllers )
    , model_      ( model )
{
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* nameLayout = new QHBoxLayout;
    name_ = new QLineEdit;
    nameLayout->addWidget( new QLabel( tr( "Name:" ) ) );
    nameLayout->addWidget( name_ );
    functions_ = new QComboBox;
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel( functions_ );
    proxy->setSourceModel( functions_->model() );
    functions_->model()->setParent( proxy );
    functions_->setModel( proxy );
    QGroupBox* descriptionGroup = new QGroupBox( tr( "Description" ) );
    descriptionGroup->setLayout( new QVBoxLayout );
    description_ = new QLabel( "---" );
    descriptionGroup->layout()->addWidget( description_ );
    timeGroup_ = new QGroupBox( tr( "Time range" ) );
    QVBoxLayout* timeLayout = new QVBoxLayout;
    timeGroup_->setLayout( timeLayout );
    timeGroup_->setCheckable( true );
    timeGroup_->setChecked( false );
    {
        QHBoxLayout* box = new QHBoxLayout;
        box->addWidget( new QLabel( tr( "First tick" ) ) );
        firstTick_ = new gui::RichSpinBox( "firstTick" );
        box->addWidget( firstTick_ );
        timeLayout->addLayout( box );
    }
    {
        QHBoxLayout* box = new QHBoxLayout;
        box->addWidget( new QLabel( tr( "Duration" ) ) );
        duration_ = new gui::RichSpinBox( "duration" );
        box->addWidget( duration_ );
        timeLayout->addLayout( box );
    }
    tools::Iterator< const AfterActionFunction& > it = model_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const AfterActionFunction& function = it.NextElement();
        functions_->addItem( function.GetDisplayName(), QVariant::fromValue( &function ) );
    }
    functions_->model()->sort( 0 );
    functions_->setCurrentIndex( 0 );
    parameters_ = new QGroupBox( tr( "Parameters" ) );
    request_ = new QPushButton( tr( "Create request" ) );
    QToolTip::add( request_, tr( "Send request" ) );
    setLayout( layout );
    layout->addLayout( nameLayout );
    layout->addWidget( functions_ );
    layout->addWidget( descriptionGroup );
    layout->addWidget( timeGroup_ );
    layout->addWidget( parameters_ );
    layout->addStretch();
    layout->addWidget( request_ );
    connect( functions_, SIGNAL( currentIndexChanged( int ) ), SLOT( OnSelectionChange( int ) ) );
    connect( request_, SIGNAL( clicked() ), SLOT( Request() ) );
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList destructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::~AfterActionFunctionList()
{
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::NotifyUpdated
// Created: JSR 2011-04-26
// -----------------------------------------------------------------------------
void AfterActionFunctionList::NotifyUpdated( const Simulation& simulation )
{
    const unsigned int tickCount = simulation.GetTickCount();
    const unsigned int firstTick = simulation.GetFirstTick();
    firstTick_->setMinValue( firstTick == std::numeric_limits< unsigned int >::max() ? tickCount : firstTick );
    firstTick_->setMaxValue( tickCount );
    duration_->setMinValue( 0 );
    duration_->setMaxValue( tickCount );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::OnSelectionChange
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionFunctionList::OnSelectionChange( int index )
{
    if( index == -1 )
        return;
    ClearParameters();
    parameters_->setLayout( new QVBoxLayout );
    builder_.SetParamInterface( *this );
    builder_.SetParentObject( this );
    const AfterActionFunction* function = 0;
    QVariant variant = functions_->itemData( index );
    if( variant.isValid() )
    {
        function = variant.value< const AfterActionFunction* >();
        if( function )
        {
            tools::Iterator< const AfterActionParameter& > it = function->CreateIterator();
            while( it.HasMoreElements() )
            {
                const AfterActionParameter& parameter = it.NextElement();
                CreateParameter( parameter );
            }
        }
    }
    parameters_->setVisible( function && function->Count() > 0 );
    QString comments = function ? function->GetComments() : QString();
    description_->setText( comments.isEmpty() ? QString( "---" ) : comments );
}

namespace
{
    struct Serializer : public actions::ParameterContainer_ABC
                      , private boost::noncopyable
    {
        explicit Serializer( IndicatorRequest& request )
            : request_( &request )
        {
            // NOTHING
        }
        virtual void AddParameter( actions::Parameter_ABC& parameter )
        {
            std::string result;
            parameter.CommitTo( result );
            request_->SetParameter( parameter.GetName().toStdString(), result );
            delete& parameter;
        }
    private:
        IndicatorRequest* request_;
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::Request
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionFunctionList::Request()
{
    int index = functions_->currentIndex();
    if( index == -1 )
        return;
    QVariant variant = functions_->itemData( index );
    if( variant.isValid() )
    {
        const AfterActionFunction* function = variant.value< const AfterActionFunction* >();
        if( function )
        {
            IndicatorRequest& request = model_.CreateRequest( *function, name_->text() );
            if( timeGroup_->isChecked() )
                request.SetTimeRange( firstTick_->value() - 1, duration_->value() );
            Serializer serializer( request );
            std::for_each( paramList_.begin(), paramList_.end(),
                boost::bind( &actions::gui::Param_ABC::CommitTo, _1, boost::ref( serializer ) ) );
            request.Commit();
            name_->clear();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< actions::gui::Param_ABC > AfterActionFunctionList::CreateParameter( const std::string& type, const QString& name )
{
    std::string compatibleType = "";
    unsigned int nbOccur = 1;
    if( type == "unit" )
        compatibleType = "agent";
    else if( type == "dotation list" )
        compatibleType = "allresourcetype";
    else if( type == "equipment list" )
        compatibleType = "maintenancepriorities";
    else if( type == "zone" )
        compatibleType = "location";
    else
    {
        nbOccur = std::numeric_limits< int >::max();
        if( type == "unit list" )
            compatibleType = "agent";
        else if( type == "crowd list" )
            compatibleType = "crowd";
        else if( type == "urban block list" )
            compatibleType = "urbanknowledge";
        else if( type == "population list" )
            compatibleType = "inhabitant";
    }
    boost::shared_ptr< actions::gui::Param_ABC > result;
    if( !compatibleType.empty() )
    {
        const kernel::OrderParameter parameter( name.toStdString(), compatibleType, false, 1, nbOccur );
        actions::gui::Param_ABC* param = &builder_.BuildOne( parameter, false );
        if( compatibleType == "location" )
            static_cast< actions::gui::ParamLocation* >( param )->SetShapeFilter( false, false, true, true, false );
        result.reset( param );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterActionFunctionList::CreateParameter( const AfterActionParameter& parameter )
{
    boost::shared_ptr< actions::gui::Param_ABC > pParameter = CreateParameter( parameter.GetType(), parameter.GetName() );
    if( pParameter )
    {
        paramList_.push_back( pParameter );
        QWidget* widget = pParameter->BuildInterface( "parameters", parameters_ );
        widget->setEnabled( true );
        pParameter->RegisterIn( controllers_.actions_ );
    }
    else
        parameters_->layout()->addWidget( new QLabel( parameter.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::ClearParameters
// Created: JSR 2013-10-17
// -----------------------------------------------------------------------------
void AfterActionFunctionList::ClearParameters()
{
    std::for_each( paramList_.begin(), paramList_.end(), boost::bind( &actions::gui::Param_ABC::RemoveFromController, _1 ) );
    paramList_.clear();
    DeleteLayout( parameters_->layout() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::Title
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
QString AfterActionFunctionList::Title() const
{
    return tr( "After action review" );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::GetIndex
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
int AfterActionFunctionList::GetIndex( actions::gui::Param_ABC* param ) const
{
    for( std::size_t i = 0; i < paramList_.size(); ++i )
        if( paramList_[ i ].get() == param )
            return static_cast< int >( i );
    return -1;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::ActivateParameters
// Created: JSR 2013-10-17
// -----------------------------------------------------------------------------
void AfterActionFunctionList::ActivateParameters( bool activate )
{
    if( activate )
        OnSelectionChange( functions_->currentIndex() );
    else
        ClearParameters();
}
