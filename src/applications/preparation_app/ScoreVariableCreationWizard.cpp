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
#include "actions_gui/ParamLocation.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/Variable.h"
#include "preparation/StaticModel.h"
#include <boost/assign/list_of.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard constructor
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
ScoreVariableCreationWizard::ScoreVariableCreationWizard( QWidget* parent, Controllers& controllers, gui::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder )
    : QDialog( parent )
    , controllers_( controllers )
    , tools_      ( tools )
    , builder_    ( builder )
{
    builder_.SetParamInterface( *this );
    builder_.SetParentObject( this );

    gui::SubObjectName subObject( "ScoreVariableCreationWizard" );
    setCaption( tr( "Create variable" ) );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 1, 5 );
    {
        gui::SubObjectName subObject( "Information" );

        QLabel* nameLabel = new QLabel( tr( "Name: " ) );
        name_ = new gui::RichLineEdit( "name" );
        connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( OnChangeName() ) );

        QLabel* typeLabel = new QLabel( tr( "Type: " ) );
        type_ = new gui::ValuedComboBox< std::string >( "type" );
        type_->AddItem( tr( "Ambulance types" ), "ambulance types" );
        type_->AddItem( tr( "Crowd states" ), "crowd states" );
        type_->AddItem( tr( "Equipment list" ), "equipment list" );
        type_->AddItem( tr( "Equipment states" ), "equipment states" );
        type_->AddItem( tr( "Fire types" ), "fire types" );
        type_->AddItem( tr( "Force ratio types" ), "force-ratio types" );
        type_->AddItem( tr( "Fratricide fires" ), "fratricide" );
        type_->AddItem( tr( "Human states" ), "human states" );
        type_->AddItem( tr( "Human ranks" ), "human ranks" );
        type_->AddItem( tr( "Maintenance types" ), "maintenance types" );
        type_->AddItem( tr( "Perception levels" ), "perception levels" );
        type_->AddItem( tr( "Population states" ), "population states" );
        type_->AddItem( tr( "Resource list" ), "dotation list" );
        type_->AddItem( tr( "Satisfaction types" ), "satisfaction types" );
        type_->AddItem( tr( "Unit" ), "unit" );
        type_->AddItem( tr( "Unit list" ), "unit list" );
        type_->AddItem( tr( "Urban blocks" ), "urban block list" );
        type_->AddItem( tr( "Zone" ), "zone" );
        type_->AddItem( tr( "Crowd" ), "crowd" );
        type_->AddItem( tr( "Crowd list" ), "crowd list" );
        type_->AddItem( tr( "Inhabitant" ), "inhabitant" );
        type_->AddItem( tr( "Inhabitant list" ), "inhabitant list" );
        connect( type_, SIGNAL( activated( int ) ), SLOT( OnChangeType() ) );

        gui::RichGroupBox* informationBox = new gui::RichGroupBox( "Information", tr( "Information" ), this );
        QVBoxLayout* informationBoxLayout = new QVBoxLayout( informationBox );
        informationBoxLayout->addWidget( nameLabel );
        informationBoxLayout->addWidget( name_ );
        informationBoxLayout->addWidget( typeLabel );
        informationBoxLayout->addWidget( type_ );
        grid->addWidget( informationBox, 0, 0 );
    }
    {
        paramBox_ = new QWidget();
        paramBox_->setLayout( new QVBoxLayout );
        grid->addWidget( paramBox_, 1, 0, 1, 2 );
    }
    {
        Q3HBox* box = new Q3HBox( this );
        ok_ = new gui::RichPushButton( "ok", tr( "Ok" ), box );
        gui::RichPushButton* cancel = new gui::RichPushButton( "cancel", tr( "Cancel" ), box );
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
            Serializer( const QString& name, const std::string& type )
            : name_( name )
            , type_( type )
        {}
        virtual void AddParameter( actions::Parameter_ABC& parameter )
        {
            indicators::DataTypeFactory types;
            std::string value;
            parameter.CommitTo( value );
            if( type_ == "circle" || type_ == "polygon" )
                type_ = "zone";

            pVariable_.reset( new indicators::Variable( name_.toStdString(), types.Instanciate( type_ ), value ) );
            delete &parameter;
        }
        const QString name_;
        std::string type_;
        std::unique_ptr< indicators::Variable > pVariable_;
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
    Serializer serializer( name_->text(), scoreType_ );
    parameter_->CommitTo( serializer );
    parameter_.reset();
    scoreType_ = "";
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
        name_->setPaletteBackgroundColor( QColor( Qt::red ).light( 120 ) );
        variableName = tr( "Variable value: " );
    }
    else
    {
        name_->unsetPalette();
        variableName = name_->text();
    }

    if( parameter_ )
        parameter_->SetName( variableName );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::OnChangeType
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::OnChangeType()
{
    if( parameter_ )
        parameter_->RemoveFromController();
    parameter_.reset();
    scoreType_ = "";

    if( paramBox_ )
    {
        delete paramBox_;
        paramBox_ = new QWidget();
        paramBox_->setLayout( new QVBoxLayout );
        static_cast< QGridLayout* >( layout() )->addWidget( paramBox_, 1, 0, 1, 2 );
    }
    gui::SubObjectName subObject( "paramBox" );

    parameter_ = CreateParameter( type_->GetValue(), name_->text() );
    if( parameter_ )
    {
        parameter_->BuildInterface( type_->GetValue().c_str(), paramBox_ );
        parameter_->SetEnabled( true );
        parameter_->RegisterIn();
    }
    else
        paramBox_->layout()->addWidget( new QLabel( name_->text() ) );
    paramBox_->show();
}

namespace
{
    std::map< std::string, std::string > AmbulanceTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Evacuation" ).toStdString(), "evacuation" )
            ( tools::translate( "ScoreDef", "Collection" ).toStdString(), "collection" );
    }

    std::map< std::string, std::string > MaintenanceTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Repairers" ).toStdString(), "repairers" )
            ( tools::translate( "ScoreDef", "Haulers" ).toStdString(), "haulers" );
    }

    std::map< std::string, std::string > ForceRatioTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Favorable" ).toStdString(), "favorable" )
            ( tools::translate( "ScoreDef", "Neutral" ).toStdString(), "neutral" )
            ( tools::translate( "ScoreDef", "Unfavorable" ).toStdString(), "unfavorable" );
    }

    std::map< std::string, std::string > CrowdStates()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Healthy" ).toStdString(), "healthy" )
            ( tools::translate( "ScoreDef", "Wounded" ).toStdString(), "wounded" )
            ( tools::translate( "ScoreDef", "Dead" ).toStdString(), "dead" )
            ( tools::translate( "ScoreDef", "Contaminated" ).toStdString(), "contaminated" );
    }

    std::map< std::string, std::string > EquipmentStates()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Available" ).toStdString(), "available" )
            ( tools::translate( "ScoreDef", "Unavailable" ).toStdString(), "unavailable" )
            ( tools::translate( "ScoreDef", "Repairable" ).toStdString(), "repairable" )
            ( tools::translate( "ScoreDef", "Repairing" ).toStdString(), "repairing" )
            ( tools::translate( "ScoreDef", "On site fixable" ).toStdString(), "on_site_fixable" )
            ( tools::translate( "ScoreDef", "Prisoner" ).toStdString(), "captured" );
    }

    std::map< std::string, std::string > FireTypes()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Direct fires" ).toStdString(), "direct" )
            ( tools::translate( "ScoreDef", "Indirect fires" ).toStdString(), "indirect" );
    }

    std::map< std::string, std::string > Fratricide()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Non fratricide fires" ).toStdString(), "false" )
            ( tools::translate( "ScoreDef", "Fratricide fires" ).toStdString(), "true" );
    }

    std::map< std::string, std::string > HumanStates()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Total" ).toStdString(), "total" )
            ( tools::translate( "ScoreDef", "Operational" ).toStdString(), "operational" )
            ( tools::translate( "ScoreDef", "Dead" ).toStdString(), "dead" )
            ( tools::translate( "ScoreDef", "Wounded" ).toStdString(), "wounded" )
            ( tools::translate( "ScoreDef", "Wounded U1" ).toStdString(), "wounded-u1" )
            ( tools::translate( "ScoreDef", "Wounded U2" ).toStdString(), "wounded-u2" )
            ( tools::translate( "ScoreDef", "Wounded U3" ).toStdString(), "wounded-u3" )
            ( tools::translate( "ScoreDef", "Wounded UE" ).toStdString(), "wounded-ue" )
            ( tools::translate( "ScoreDef", "Mental" ).toStdString(), "mental" )
            ( tools::translate( "ScoreDef", "Nbc" ).toStdString(), "nbc" )
            ( tools::translate( "ScoreDef", "In treatment" ).toStdString(), "in-treatment" )
            ( tools::translate( "ScoreDef", "In maintenance" ).toStdString(), "in-maintenance" );
    }

    std::map< std::string, std::string > HumanRanks()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Officer" ).toStdString(), "officer" )
            ( tools::translate( "ScoreDef", "Sub officer" ).toStdString(), "sub-officer" )
            ( tools::translate( "ScoreDef", "Troopers" ).toStdString(), "troopers" );
    }

   std::map< std::string, std::string > PerceptionLevels()
    {
        return boost::assign::map_list_of( tools::translate( "ScoreDef", "Invisible" ).toStdString(), "invisible" )
            ( tools::translate( "ScoreDef", "Detected" ).toStdString(), "detected" )
            ( tools::translate( "ScoreDef", "Recognized" ).toStdString(), "recognized" )
            ( tools::translate( "ScoreDef", "Identified" ).toStdString(), "identified" )
            ( tools::translate( "ScoreDef", "Recorded" ).toStdString(), "recorded" );
    }

   std::map< std::string, std::string > PopulationStates()
   {
       return boost::assign::map_list_of( tools::translate( "ScoreDef", "Healthy" ).toStdString(), "healthy" )
           ( tools::translate( "ScoreDef", "Wounded" ).toStdString(), "wounded" )
           ( tools::translate( "ScoreDef", "Dead" ).toStdString(), "dead" );
   }

   std::map< std::string, std::string > SatisfactionTypes()
   {
       return boost::assign::map_list_of( tools::translate( "ScoreDef", "Lodging" ).toStdString(), "lodging" )
           ( tools::translate( "ScoreDef", "Health" ).toStdString(), "health" )
           ( tools::translate( "ScoreDef", "Safety" ).toStdString(), "safety" );
   }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::CreateParameter
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
boost::shared_ptr< actions::gui::Param_ABC > ScoreVariableCreationWizard::CreateParameter( const std::string& type, const QString& name )
{
    scoreType_ = type;
    std::string compatibleType = "";
    unsigned int nbOccur = std::numeric_limits< unsigned int >::max();

    if( type == "unit list" )
        compatibleType = "agent";
    else if( type == "urban block list" )
        compatibleType = "urbanknowledge";
    else if( type == "crowd list" )
        compatibleType = "crowd";
    else if( type == "inhabitant list" )
        compatibleType = "inhabitant";
    else
    {
        nbOccur = 1;
        if( type == "unit" )
            compatibleType = "agent";
        if( type == "crowd" )
            compatibleType = "crowd";
        if( type == "inhabitant" )
            compatibleType = "inhabitant";
        if( type == "dotation list" )
            compatibleType = "allresourcetype";
        else if( type == "equipment list" )
            compatibleType = "maintenancepriorities";
        else if( type == "zone" )
            compatibleType = "location";
    }

    const QString variableName = name.isEmpty() ? tr( "Variable value: " ) : name;
    boost::shared_ptr< actions::gui::Param_ABC > result;

    if( !compatibleType.empty() )
    {
        const OrderParameter parameter( variableName.toStdString(), compatibleType, false, 1, nbOccur );
        actions::gui::Param_ABC* param = &builder_.BuildOne( parameter, false );
        if( compatibleType == "location" )
            static_cast< actions::gui::ParamLocation* >( param )->SetShapeFilter( false, false, true, true, false );
        result.reset( param );
    }
    else
    {
        // TODO: move ParamStringEnumeration to actions_gui and make it creatable by the builder
        const OrderParameter parameter( variableName.toStdString(), type, false );
        if( type == "force-ratio types" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Force ratio types" ),  parameter, ForceRatioTypes() ) );
        else if( type == "ambulance types" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Ambulance types" ),    parameter, AmbulanceTypes() ) );
        else if( type == "crowd states" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Crowd states" ),       parameter, CrowdStates() ) );
        else if( type == "equipment states" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Equipment states" ),   parameter, EquipmentStates() ) );
        else if( type == "fire types" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Fire types" ),         parameter, FireTypes() ) );
        else if( type == "fratricide" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Fratricide fires" ),   parameter, Fratricide() ) );
        else if( type == "human states" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Human states" ),       parameter, HumanStates() ) );
        else if( type == "human ranks" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Human ranks" ),        parameter, HumanRanks() ) );
        else if( type == "maintenance types" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Maintenance types" ),  parameter, MaintenanceTypes() ) );
        else if( type == "population states" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Population states" ),  parameter, PopulationStates() ) );
        else if( type == "perception levels" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Perception levels" ),  parameter, PerceptionLevels() ) );
        else if( type == "satisfaction types" )
            result.reset( new ParamStringEnumeration( builder_, tr( "Satisfaction types" ), parameter, SatisfactionTypes() ) );
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

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::Draw
// Created: FPO 2011-07-19
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::Draw( gui::Viewport_ABC& viewport )
{
    if( parameter_ && isVisible() )
        parameter_->Draw( geometry::Point2f(), viewport, tools_ );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::Title
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
QString ScoreVariableCreationWizard::Title() const
{
    return tr( "Create variable" );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::GetIndex
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
int ScoreVariableCreationWizard::GetIndex( actions::gui::Param_ABC* /*param*/ ) const
{
    return 1;
}
