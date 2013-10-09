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

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList constructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, AfterActionModel& model, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : QWidget( parent )
    , builder_    ( interfaceBuilder )
    , controllers_( controllers )
    , model_      ( model )
    , request_    ( 0 )
{
    QVBoxLayout* layout = new QVBoxLayout;
    functions_ = new QTreeWidget;
    functions_->setHeaderLabel( tr( "Name" ) );
    functions_->setRootIsDecorated( false );
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
    connect( functions_, SIGNAL( itemSelectionChanged() ), SLOT( OnSelectionChange() ) );

    tools::Iterator< const AfterActionFunction& > it = model_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const AfterActionFunction& function = it.NextElement();
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText( 0, function.GetName() );
        item->setToolTip( 0, function.GetComments() );
        item->setData( 0, Qt::UserRole, QVariant::fromValue( &function ) );
        functions_->addTopLevelItem( item );
    }
    parameters_ = new QGroupBox( tr( "Parameters" ) );
    setLayout( layout );
    layout->addWidget( functions_ );
    layout->addWidget( timeGroup_ );
    layout->addWidget( parameters_ );
    CreateRequestButton();
    request_->setEnabled( false );
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
// Name: AfterActionFunctionList::CreateRequestButton
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionFunctionList::CreateRequestButton()
{
    delete request_;
    request_ = new QPushButton( tr( "Request" ) );
    layout()->addWidget( request_ );
    QToolTip::add( request_, tr( "Send request" ) );
    connect( request_, SIGNAL( clicked() ), SLOT( Request() ) );
    request_->show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::OnSelectionChange
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionFunctionList::OnSelectionChange()
{
    delete request_;
    request_ = 0;
    std::for_each( paramList_.begin(), paramList_.end(), boost::bind( &actions::gui::Param_ABC::RemoveFromController, _1 ) );
    paramList_.clear();
    delete parameters_;
    parameters_ = new QGroupBox( tr( "Parameters" ) );
    parameters_->setLayout( new QVBoxLayout );
    layout()->addWidget( parameters_ );
    builder_.SetParamInterface( *this );
    builder_.SetParentObject( this );
    QTreeWidgetItem* selected = functions_->currentItem();
    if( selected && selected->data( 0, Qt::UserRole ).isValid() )
    {
        const AfterActionFunction* function = selected->data( 0, Qt::UserRole ).value< const AfterActionFunction* >();
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
    parameters_->show();
    CreateRequestButton();
    request_->setEnabled( selected != 0 );
}

namespace
{
    struct Serializer : public actions::ParameterContainer_ABC
                      , private boost::noncopyable
    {
        Serializer( IndicatorRequest& request )
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
    QTreeWidgetItem* selected = functions_->currentItem();
    if( selected && selected->data( 0, Qt::UserRole ).isValid() )
    {
        const AfterActionFunction* function = selected->data( 0, Qt::UserRole ).value< const AfterActionFunction* >();
        if( function )
        {
            IndicatorRequest& request = model_.CreateRequest( *function );
            if( timeGroup_->isChecked() )
                request.SetTimeRange( firstTick_->value() - 1, duration_->value() );
            Serializer serializer( request );
            std::for_each( paramList_.begin(), paramList_.end(),
                boost::bind( &actions::gui::Param_ABC::CommitTo, _1, boost::ref( serializer ) ) );
            request.Commit();
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
        pParameter->BuildInterface( "parameters", parameters_ );
        pParameter->RegisterIn( controllers_.actions_ );
    }
    else
        parameters_->layout()->addWidget( new QLabel( parameter.GetName() ) );
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
