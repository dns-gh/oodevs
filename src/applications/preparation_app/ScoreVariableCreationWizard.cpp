// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreVariableCreationWizard.h"
#include "moc_ScoreVariableCreationWizard.cpp"
#include "ParamStringEnumeration.h"
#include "actions/Parameter_ABC.h"
#include "actions/ParameterContainer_ABC.h"
#include "actions_gui/ParamAgent.h"
#include "actions_gui/ParamAgentList.h"
#include "actions_gui/ParamDotationTypeList.h"
#include "actions_gui/ParamEquipmentList.h"
#include "actions_gui/ParamLocation.h"
#include "actions_gui/ParamUrbanBlockList.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderParameter.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/Variable.h"
#include "clients_kernel/Tools.h"
#include "preparation/StaticModel.h"
#include <boost/assign/list_of.hpp>
#include <qvgroupbox.h>

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard constructor
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
ScoreVariableCreationWizard::ScoreVariableCreationWizard( QWidget* parent, kernel::Controllers& controllers, gui::ParametersLayer& layer, const StaticModel& staticModel )
    : QDialog( parent )
    , controllers_( controllers )
    , layer_( layer )
    , staticModel_( staticModel )
{
    setCaption( tr( "Create variable" ) );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 1, 5 );
    {
        QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Information" ), this );
        new QLabel( tr( "Name: " ), box );
        name_ = new QLineEdit( box );
        connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( OnChangeName() ) );
        new QLabel( tr( "Type: " ), box );
        type_ = new gui::ValuedComboBox< std::string >( box );
        type_->AddItem( tr( "Ambulance types" ), "ambulance types" );
        type_->AddItem( tr( "Crowd states" ), "crowd states" );
        type_->AddItem( tr( "Equipment list" ), "equipment list" );
        type_->AddItem( tr( "Equipment states" ), "equipment states" );
        type_->AddItem( tr( "Fire types" ), "fire types" );
        type_->AddItem( tr( "Fratricide fires" ), "fratricide" );
        type_->AddItem( tr( "Human states" ), "human states" );
        type_->AddItem( tr( "Human ranks" ), "human ranks" );
        type_->AddItem( tr( "Perception levels" ), "perception levels" );
        type_->AddItem( tr( "Population states" ), "population states" );
        type_->AddItem( tr( "Resource list" ), "dotation list" );
        type_->AddItem( tr( "Satisfaction types" ), "satisfaction types" );
        type_->AddItem( tr( "Unit" ), "unit" );
        type_->AddItem( tr( "Unit list" ), "unit list" );
        type_->AddItem( tr( "Urban blocks" ), "urban block list" );
        type_->AddItem( tr( "Zone" ), "zone" );

        connect( type_, SIGNAL( activated( int ) ), SLOT( OnChangeType() ) );
        grid->addWidget( box, 0, 0 );
    }
    {
        paramBox_ = new QVGroupBox( tr( "Value" ), this );
        grid->addWidget( paramBox_, 1, 0 );
    }
    {
        QHBox* box = new QHBox( this );
        ok_ = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 2, 0 );
        connect( ok_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    }
    hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard destructor
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
ScoreVariableCreationWizard::~ScoreVariableCreationWizard()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::Create
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::Create()
{
    name_->clear();
    OnChangeName();
    OnChangeType();
    show();
}

namespace
{
    struct Serializer : public actions::ParameterContainer_ABC
    {
        explicit Serializer( const QString& name )
            : name_( name )
        {}
        virtual void AddParameter( actions::Parameter_ABC& parameter )
        {
            indicators::DataTypeFactory types;
            std::string value;
            parameter.CommitTo( value );
            pVariable_.reset( new indicators::Variable( name_.ascii(), types.Instanciate( parameter.GetType() ), value ) );
            delete &parameter;
        }
        const QString name_;
        std::auto_ptr< indicators::Variable > pVariable_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::OnAccept
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::OnAccept()
{
    if( name_->text().isEmpty() )
        return;
    if( parameter_.get() )
        parameter_->RemoveFromController();
    Serializer serializer( name_->text() );
    parameter_->CommitTo( serializer );
    emit VariableCreated( *serializer.pVariable_ );
    accept();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::hideEvent
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::hideEvent( QHideEvent* e )
{
    QDialog::hideEvent( e );
    emit Closed();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::OnChangeName
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::OnChangeName()
{
    QString variableName;

    ok_->setDisabled( name_->text().isEmpty() );

    if( name_->text().isEmpty() )
    {
        name_->setPaletteBackgroundColor( Qt::red.light( 120 ) );
        variableName = tr( "Variable value: " );
    }
    else
    {
        name_->unsetPalette();
        variableName = name_->text();
    }

    if( parameter_ )
    {
        if( type_->GetValue() == "unit" )
        {
            static_cast< actions::gui::ParamAgent* >( parameter_.get() )->SetName( variableName );
        }
        else if( type_->GetValue() == "unit list" )
        {
            static_cast< actions::gui::ParamAgentList* >( parameter_.get() )->SetName( variableName );
        }
        else if( type_->GetValue() == "zone" )
        {
            static_cast< actions::gui::ParamLocation* >( parameter_.get() )->SetName( variableName );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::OnChangeType
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::OnChangeType()
{
    if( parameter_ )
    {
        parameter_->RemoveFromController(); parameter_.reset();
        delete paramBox_;
        paramBox_ = new QVGroupBox( tr( "Value" ), this );
        static_cast< QGridLayout* >( layout() )->addWidget( paramBox_, 1, 0 );
    }
    parameter_ = CreateParameter( type_->GetValue(), name_->text() );
    if( parameter_ )
    {
        parameter_->BuildInterface( paramBox_ );
        parameter_->RegisterIn( controllers_.actions_ );
    }
    else
        new QLabel( name_->text(), paramBox_ );
    paramBox_->show();
}

namespace
{
    std::map< std::string, std::string > AmbulanceTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Evacuation" ).ascii(), "evacuation" )
            ( tools::translate( "ScoreDef", "Collection" ).ascii(), "collection" );
    }

    std::map< std::string, std::string > CrowdStates()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Healthy" ).ascii(), "healthy" )
            ( tools::translate( "ScoreDef", "Wounded" ).ascii(), "wounded" )
            ( tools::translate( "ScoreDef", "Dead" ).ascii(), "dead" )
            ( tools::translate( "ScoreDef", "Contaminated" ).ascii(), "contaminated" );
    }

    std::map< std::string, std::string > EquipmentStates()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Available" ).ascii(), "available" )
            ( tools::translate( "ScoreDef", "Unavailable" ).ascii(), "unavailable" )
            ( tools::translate( "ScoreDef", "Repairable" ).ascii(), "repairable" )
            ( tools::translate( "ScoreDef", "Repairing" ).ascii(), "repairing" )
            ( tools::translate( "ScoreDef", "Prisoner" ).ascii(), "prisoner" );
    }

    std::map< std::string, std::string > FireTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Direct fires" ).ascii(), "direct" )
            ( tools::translate( "ScoreDef", "Indirect fires" ).ascii(), "indirect" );
    }

    std::map< std::string, std::string > Fratricide()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Non fratricide fires" ).ascii(), "false" )
            ( tools::translate( "ScoreDef", "Fratricide fires" ).ascii(), "true" );
    }

    std::map< std::string, std::string > HumanStates()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Total" ).ascii(), "total" )
            ( tools::translate( "ScoreDef", "Operational" ).ascii(), "operational" )
            ( tools::translate( "ScoreDef", "Dead" ).ascii(), "dead" )
            ( tools::translate( "ScoreDef", "Wounded" ).ascii(), "wounded" )
            ( tools::translate( "ScoreDef", "Mental" ).ascii(), "mental" )
            ( tools::translate( "ScoreDef", "Nbc" ).ascii(), "nbc" )
            ( tools::translate( "ScoreDef", "In treatment" ).ascii(), "in-treatment" )
            ( tools::translate( "ScoreDef", "In maintenance" ).ascii(), "in-maintenance" );
    }

    std::map< std::string, std::string > HumanRanks()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Officer" ).ascii(), "officer" )
            ( tools::translate( "ScoreDef", "Sub officer" ).ascii(), "sub-officer" )
            ( tools::translate( "ScoreDef", "Troopers" ).ascii(), "troopers" );
    }

    std::map< std::string, std::string > MaintenanceTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Repairers" ).ascii(), "repairers" )
            ( tools::translate( "ScoreDef", "Haulers" ).ascii(), "haulers" );
    }

   std::map< std::string, std::string > PerceptionLevels()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Invisible" ).ascii(), "invisible" )
            ( tools::translate( "ScoreDef", "Detected" ).ascii(), "detected" )
            ( tools::translate( "ScoreDef", "Recognized" ).ascii(), "recognized" )
            ( tools::translate( "ScoreDef", "Identified" ).ascii(), "identified" )
            ( tools::translate( "ScoreDef", "Recorded" ).ascii(), "recorded" );
    }

   std::map< std::string, std::string > PopulationStates()
   {
       return boost::assign::map_list_of( tools::translate( "ScoreDef", "Healthy" ).ascii(), "healthy" )
           ( tools::translate( "ScoreDef", "Wounded" ).ascii(), "wounded" )
           ( tools::translate( "ScoreDef", "Dead" ).ascii(), "dead" );
   }

   std::map< std::string, std::string > SatisfactionTypes()
   {
       return boost::assign::map_list_of( tools::translate( "ScoreDef", "Lodging" ).ascii(), "lodging" )
           ( tools::translate( "ScoreDef", "Health" ).ascii(), "health" )
           ( tools::translate( "ScoreDef", "Safety" ).ascii(), "safety" );
   }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::CreateParameter
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
boost::shared_ptr< actions::gui::Param_ABC > ScoreVariableCreationWizard::CreateParameter( const std::string& type, const QString& name )
{
    const QString variableName = name.isEmpty() ? tr( "Variable value: " ) : name;
    boost::shared_ptr< actions::gui::Param_ABC > result;
    if( type == "unit list" )
    {
        const kernel::OrderParameter parameter( variableName.ascii(), type.c_str(), false, 1, std::numeric_limits< unsigned int >::max() );
        result.reset( new actions::gui::ParamAgentList( this, parameter, controllers_.actions_, controllers_.controller_ ) );
    }
    else
    {
        const kernel::OrderParameter parameter( variableName.ascii(), type.c_str(), false );

        if( type == "unit" )
            result.reset( new actions::gui::ParamAgent( this, parameter, controllers_.controller_ ) );
        else if( type == "dotation list" )
            result.reset( new actions::gui::ParamDotationTypeList( this, parameter, staticModel_.objectTypes_ ) );
        else if( type == "equipment list" )
            result.reset( new actions::gui::ParamEquipmentList( this, parameter, staticModel_.objectTypes_ ) );
        else if( type == "urban block list" )
            result.reset( new actions::gui::ParamUrbanBlockList( this, parameter, controllers_.actions_, controllers_.controller_ ) );
        else if( type == "zone" )
        {
            std::auto_ptr< actions::gui::ParamLocation > location( new actions::gui::ParamLocation( parameter, layer_, staticModel_.coordinateConverter_ ) );
            location->SetShapeFilter( false, false, true, true, false );
            result.reset( location.release() );
        }
        else if( type == "ambulance types" )
            result.reset( new ParamStringEnumeration( this, tr( "Ambulance types" ), parameter, AmbulanceTypes() ) );
        else if( type == "crowd states" )
            result.reset( new ParamStringEnumeration( this, tr( "Crowd states" ), parameter, CrowdStates() ) );
        else if( type == "equipment states" )
            result.reset( new ParamStringEnumeration( this, tr( "Equipment states" ), parameter, EquipmentStates() ) );
        else if( type == "fire types" )
            result.reset( new ParamStringEnumeration( this, tr( "Fire types" ), parameter, FireTypes() ) );
        else if( type == "fratricide" )
            result.reset( new ParamStringEnumeration( this, tr( "Fratricide fires" ), parameter, Fratricide() ) );
        else if( type == "human states" )
            result.reset( new ParamStringEnumeration( this, tr( "Human states" ), parameter, HumanStates() ) );
        else if( type == "human ranks" )
            result.reset( new ParamStringEnumeration( this, tr( "Human ranks" ), parameter, HumanRanks() ) );
        else if( type == "maintenance types" )
            result.reset( new ParamStringEnumeration( this, tr( "Maintenance types" ), parameter, MaintenanceTypes() ) );
        else if( type == "population states" )
            result.reset( new ParamStringEnumeration( this, tr( "Population states" ), parameter, PopulationStates() ) );
        else if( type == "perception levels" )
            result.reset( new ParamStringEnumeration( this, tr( "Perception levels" ), parameter, PerceptionLevels() ) );
        else if( type == "satisfaction types" )
            result.reset( new ParamStringEnumeration( this, tr( "Satisfaction types" ), parameter, SatisfactionTypes() ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::sizeHint
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
QSize ScoreVariableCreationWizard::sizeHint() const
{
    return QSize( 200, 200 );
}
