// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_DetectionAlgorithmPrevision.h"
#include "moc_ADN_Sensors_DetectionAlgorithmPrevision.cpp"
#include "ADN_GuiBuilder.h"
#include "ADN_Sensors_GUI.h"
#include "ADN_Graph.h"
#include "ADN_Connector_Graph_ABC.h"
#include "ADN_GraphData.h"
#include "GQ_PlotData.h"
#include <boost/lexical_cast.hpp>

namespace
{
    class PerceptionConverter : public GQ_ValueConvertor< double, QString >
    {
    public:
        virtual void Convert( double input, QString& output ) const
        {
            if( input == 3. )
                output = "Identified";
            else if( input == 2. )
                output = "Recognized";
            else if( input == 1. )
                output = "Detected";
            else if( input == 0. )
                output = "Unseen";
        }
        // -----------------------------------------------------------------------------
        // Name: GetConversion
        // Created: HBD 2010-05-04
        // -----------------------------------------------------------------------------
        QString GetConversion( double input ) const
        {
            if( input >= 3. )
                return "Identified";
            else if( input >= 2. )
                return "Recognized";
            else if( input >= 1. )
                return "Detected";
            else if( input >= 0. )
                return "Unseen";
        }
    };

    PerceptionConverter convertor;

    GQ_PlotData* CreateGraphData( unsigned int hue, unsigned int valence, GQ_Plot* pGraph )
    {
        GQ_PlotData* plotData = new GQ_PlotData( 0, *pGraph );
        QColor color;
        color.setHsv( hue, 255, valence );
        GQ_PlotData::E_PointShapeType nPointShape = GQ_PlotData::eDot;
        plotData->SetPointShape( nPointShape );
        plotData->SetPointPen( QPen( color ) );
        plotData->SetLinePen( QPen( color ) );
        pGraph->RegisterPlotData( *plotData );
        return plotData;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision constructor
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
ADN_Sensors_DetectionAlgorithmPrevision::ADN_Sensors_DetectionAlgorithmPrevision( QWidget* parent )
    : QGroupBox( 2, Qt::Vertical, tr( "Simulation" ), parent )
    , perceiverPostureFactor_ ( 1. )
    , targetPostureFactor_    ( 1. )
    , sizeFactor_             ( 1. )
    , weatherFactor_          ( 1. )
    , illuminationFactor_     ( 1. )
    , environmentFactor_      ( 1. )
    , populationDensityFactor_( 1. )
    , populationModifier_     ( 1. )
    , population_             ( 0. )
    , urbanHeightRatio_       ( 1. )
    , urbanOccupation_        ( 1. )
    , urbanSelected_          ( false )
{
    {
        QGrid* group = new QGrid ( 2,this );
        group->setSpacing( 5 );
        new QLabel( tr( "Stance" ), group );
        stance_ = new QLineEdit( group, tr( "Stance" ) );
        stance_->setPaletteBackgroundColor( Qt::lightGray );
        stance_->setReadOnly( true );
        new QLabel( tr( "Target Stance" ), group );
        stanceTarget_ = new QLineEdit( group, tr( "Stance Target" ) );
        stanceTarget_->setReadOnly( true );
        stanceTarget_->setPaletteBackgroundColor( Qt::lightGray );
        new QLabel( tr( "Volume" ), group );
        volume_ = new QLineEdit( group, tr( "Volume" ) );
        volume_->setReadOnly( true );
        volume_->setPaletteBackgroundColor( Qt::lightGray );
        new QLabel( tr( "Weather" ), group);
        weather_= new QLineEdit( group, tr( "Weather" ) );
        weather_->setReadOnly( true );
        weather_->setPaletteBackgroundColor( Qt::lightGray );
        new QLabel( tr( "Illumination" ), group );
        illumnination = new QLineEdit( group, tr( "Illumination" ) );
        illumnination->setReadOnly( true );
        illumnination->setPaletteBackgroundColor( Qt::lightGray );
        new QLabel( tr( "Environment" ), group );
        environment_= new QLineEdit( group, tr( "Environment" ) );
        environment_->setReadOnly( true );
        environment_->setPaletteBackgroundColor( Qt::lightGray );
        new QLabel( tr( "Urban Material" ), group );
        urbanMaterial_= new QLineEdit( group, tr( "Urban Material" ) );
        urbanMaterial_->setReadOnly( true );
        urbanMaterial_->setPaletteBackgroundColor( Qt::lightGray );
        new QLabel( tr( "Population Value" ), group );
        populationValue_ = new QLineEdit( group, tr( "Population Value" ) );
        populationValue_->setText( "0" );
        populationValue_->setValidator( new QDoubleValidator( populationValue_ ) );
        connect( populationValue_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnPopulationChanged( const QString& ) ) );
        connect( populationValue_, SIGNAL( returnPressed() ), this, SLOT( UpdateValue() ) );
        connect( populationValue_, SIGNAL( lostFocus() ), this, SLOT( UpdateValue() ) );
        new QLabel( tr( "Urban Height Ratio" ), group );
        urbanHeightRatioValue_ = new QLineEdit( group, tr( "Urban Height Ratio" ) );
        urbanHeightRatioValue_->setText( "1" );
        urbanHeightRatioValue_->setValidator( new QDoubleValidator( 0., 1., 0, urbanHeightRatioValue_ ) );
        connect( urbanHeightRatioValue_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUrbanHeightRatioChanged( const QString& ) ) );
        connect( urbanHeightRatioValue_, SIGNAL( returnPressed() ), this, SLOT( UpdateValue() ) );
        connect( urbanHeightRatioValue_, SIGNAL( lostFocus() ), this, SLOT( UpdateValue() ) );
        new QLabel( tr( "Urban Occupation Value" ), group );
        urbanOccupationValue_ = new QLineEdit( group, tr( "Urban Occupation Value" ) );
        urbanOccupationValue_->setText( "1" );
        urbanOccupationValue_->setValidator( new QDoubleValidator( 0., 1., 0, urbanOccupationValue_ ) );
        connect( urbanOccupationValue_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUrbanOccupationChanged( const QString& ) ) );
        connect( urbanOccupationValue_, SIGNAL( returnPressed() ), this, SLOT( UpdateValue() ) );
        connect( urbanOccupationValue_, SIGNAL( lostFocus() ), this, SLOT( UpdateValue() ) );
    }

    GQ_Plot* pGraph = new GQ_Plot( this );
    pGraph->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    pGraph->setMinimumSize( 400, 200 );
    GQ_PlotAxis& yAxis = pGraph->YAxis();
    yAxis.SetAxisCaption( tr( "Perception" ) );
    yAxis.SetAxisRange( 0, 3, true );
    yAxis.SetTicksLength( 2 );
    yAxis.ShowTicks( 2 );
    yAxis.ShowSubTicks( false );
    yAxis.ShowTicksValue( true );
    yAxis.SetValueToStringConvertor( &convertor );

    GQ_PlotAxis& xAxis = pGraph->XAxis();
    xAxis.SetAxisCaption( tr( "Distance (m)" ) );
    xAxis.ShowTicks( 50 );
    xAxis.ShowSubTicks( 10 );
    xAxis.ShowTicksValue( true );

    pGraphData_ = CreateGraphData( 0, 255, pGraph );
    pBaseGraphData_ = CreateGraphData( 166, 0, pGraph );

    for( unsigned int i = 0; i < 7; ++i )
    {
        pGraphData_->AddPoint( 0., 0. );
        pBaseGraphData_->AddPoint( 0., 0. );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision destructor
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
ADN_Sensors_DetectionAlgorithmPrevision::~ADN_Sensors_DetectionAlgorithmPrevision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPerceiverStanceChanged
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPerceiverStanceChanged( std::string posture, double factor )
{
    perceiverPostureFactor_ = factor;
    stance_->setText( posture.c_str() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnTargetStanceChanged
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnTargetStanceChanged( std::string posture, double factor )
{
    targetPostureFactor_ = factor;
    stanceTarget_->setText( posture.c_str() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::Update
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::Update()
{
    double population = population_ ? std::min( 1., populationModifier_ * populationDensityFactor_ / population_ ) : 1;
    double environmentFactor = urbanSelected_ ? 1 + urbanOccupation_ * ( environmentFactor_ * urbanHeightRatio_ - 1 ) : environmentFactor_;
    if( environmentFactor > 1 )
        environmentFactor = 1;
    double value = perceiverPostureFactor_ * targetPostureFactor_ * sizeFactor_ * weatherFactor_ *
                   illuminationFactor_* environmentFactor * population;

    pBaseGraphData_->ChangePoint( 0, std::make_pair( 0., 3. ) );
    pBaseGraphData_->ChangePoint( 1, std::make_pair( identification_, 3. ) );
    pBaseGraphData_->ChangePoint( 2, std::make_pair( identification_, 2. ) );
    pBaseGraphData_->ChangePoint( 3, std::make_pair( recognition_, 2. ) );
    pBaseGraphData_->ChangePoint( 4, std::make_pair( recognition_, 1. ) );
    pBaseGraphData_->ChangePoint( 5, std::make_pair( detection_, 1. ) );
    pBaseGraphData_->ChangePoint( 6, std::make_pair( detection_, 0. ) );
    pBaseGraphData_->TouchRange();
    pBaseGraphData_->TouchData();

    double detection = detection_ * value;
    double recognition = recognition_ * value;
    double identification = identification_ * value;

    pGraphData_->ChangePoint( 0, std::make_pair( 0., 3. ) );
    pGraphData_->ChangePoint( 1, std::make_pair( identification, 3. ) );
    pGraphData_->ChangePoint( 2, std::make_pair( identification, 2. ) );
    pGraphData_->ChangePoint( 3, std::make_pair( recognition, 2. ) );
    pGraphData_->ChangePoint( 4, std::make_pair( recognition, 1. ) );
    pGraphData_->ChangePoint( 5, std::make_pair( detection, 1. ) );
    pGraphData_->ChangePoint( 6, std::make_pair( detection, 0. ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnSelectSensor
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnSelectSensor( void* data )
{
    if( ADN_Sensors_Data::SensorInfos* infos = static_cast< ADN_Sensors_Data::SensorInfos* >( data ) )
    {
        detection_ = infos->rDistDetection_.GetData();
        recognition_ = infos->rDistReco_.GetData();
        identification_ = infos->rDistIdent_.GetData();
        sizeFactor_ = 1;
        perceiverPostureFactor_ = 1;
        targetPostureFactor_= 1;
        sizeFactor_= 1;
        weatherFactor_= 1;
        illuminationFactor_= 1;
        environmentFactor_= 1;
        populationModifier_ = infos->populationInfos_.rModifier_.GetData();
        populationDensityFactor_ = infos->populationInfos_.rDensity_.GetData();
        Update();
        volume_->clear();
        stance_->clear();
        stanceTarget_->clear();
        weather_->clear();
        illumnination->clear();
        environment_->clear();
        urbanMaterial_->clear();
        populationValue_->setText( "0" );
        urbanHeightRatioValue_->setText( "1" );
        urbanOccupationValue_->setText( "1" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnSizeChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnSizeChanged( std::string volume, double factor )
{
    sizeFactor_ = factor;
    volume_->setText( volume.c_str() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::WeatherChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::WeatherChanged( std::string weather, double factor )
{
    weatherFactor_ = factor;
    weather_->setText( weather.c_str() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::IlluminationChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::IlluminationChanged( std::string illumination, double factor )
{
    illuminationFactor_ = factor;
    illumnination->setText( illumination.c_str() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::EnvironmentChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::EnvironmentChanged( ADN_Sensors_Data::ModificatorEnvironmentInfos* env, double factor )
{
    environment_->setText( env->GetItemName().c_str() );
    urbanMaterial_->setText( tr( "Cannot be selected." ) );
    urbanSelected_ = false;
    environmentFactor_ = factor;
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::UrbanBlockChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::UrbanBlockChanged( std::string material, double factor )
{
    urbanMaterial_->setText( material.c_str() );
    environment_->setText( tr( "Cannot be selected." ) );
    urbanSelected_ = true;
    environmentFactor_ = factor;
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnDetectionChanged
// Created: HBD 2010-05-04
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnDetectionChanged( const QString& value )
{
    if( !value.isEmpty() )
        try
    {
        detection_ = boost::lexical_cast< double >( value.ascii() );
    }
    catch( boost::bad_lexical_cast& /*e*/ )
    {
        detection_ = 0;

    }        
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnRecognitionChanged
// Created: HBD 2010-05-04
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnRecognitionChanged( const QString& value )
{
    if( !value.isEmpty() )
        try
    {
        recognition_ = boost::lexical_cast< double >( value.ascii() );
    }
    catch( boost::bad_lexical_cast& /*e*/ )
    {
        recognition_ = 0; 
    } 
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnIdentificationChanged
// Created: HBD 2010-05-04
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnIdentificationChanged( const QString& value )
{
    if( !value.isEmpty() )
            try
        {
            identification_ = boost::lexical_cast< double >( value.ascii() );
        }
        catch( boost::bad_lexical_cast& /*e*/ )
        {
            identification_ = 0; 
        } 
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationDensityChanged
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationDensityChanged( const QString& value )
{
    if( !value.isEmpty() )
        try
        {
             populationDensityFactor_ = boost::lexical_cast< double >( value.ascii() );
        }
       catch( boost::bad_lexical_cast& /*e*/ )
        {
            populationDensityFactor_ = 1;
        }
    else
        populationDensityFactor_ = 1;
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationModifierChanged
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationModifierChanged( const QString& value )
{
    if( !value.isEmpty() )
        try
        {
            populationModifier_ = boost::lexical_cast< double >( value.ascii() );
        }
      catch( boost::bad_lexical_cast& /*e*/ )
         {
            populationModifier_ = 1;
         }
    else
        populationModifier_ = 1;
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationChanged
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationChanged( const QString& value )
{
    UpdatePreview( population_, populationValue_, 0., "0", value );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanHeightRatioChanged
// Created: LDC 2010-08-16
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanHeightRatioChanged( const QString& value )
{
    UpdatePreview( urbanHeightRatio_, urbanHeightRatioValue_, 1., "1", value );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanOccupationChanged
// Created: LDC 2010-08-16
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanOccupationChanged( const QString& value )
{
    UpdatePreview( urbanOccupation_, urbanOccupationValue_, 1., "1", value );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::UpdatePreview
// Created: LDC 2010-08-16
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::UpdatePreview( double& parameter, QLineEdit* widget, double defaultValue, const QString& defaultString, const QString& value )
{
    if( !value.isEmpty() )
       try
       {
           parameter = boost::lexical_cast< double >( value.ascii() );
       }
       catch( boost::bad_lexical_cast& /*e*/ )
       {
           widget->setText( defaultString );
           parameter = defaultValue;
       }
    else
        parameter = defaultValue;
    Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::UpdateValue
// Created: LGY 2010-08-12
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::UpdateValue()
{
    populationValue_->setText( boost::lexical_cast< std::string >( population_ ).c_str() );
    urbanHeightRatioValue_->setText( boost::lexical_cast< std::string >( urbanHeightRatio_ ).c_str() );
    urbanOccupationValue_->setText( boost::lexical_cast< std::string >( urbanOccupation_ ).c_str() );
}
