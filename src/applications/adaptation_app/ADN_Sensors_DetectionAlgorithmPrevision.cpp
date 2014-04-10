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

#include "clients_gui/GQ_Plot.h"
#include "clients_gui/GQ_ValueConvertor.h"

namespace
{
    class PerceptionConverter : public gui::GQ_ValueConvertor< double, QString >
    {
    public:
        virtual void Convert( double input, QString& output ) const
        {
            if( !qApp )
                return;
            if( input >= 3. )
                output = qApp->translate( "ADN_Sensors_DetectionAlgorithmPrevision", "Identified" );
            else if( input >= 2. )
                output = qApp->translate( "ADN_Sensors_DetectionAlgorithmPrevision", "Recognized" );
            else if( input >= 1. )
                output = qApp->translate( "ADN_Sensors_DetectionAlgorithmPrevision", "Detected" );
            else if( input >= 0. )
                output = qApp->translate( "ADN_Sensors_DetectionAlgorithmPrevision", "Unseen" );
        }
        // -----------------------------------------------------------------------------
        // Name: GetConversion
        // Created: HBD 2010-05-04
        // -----------------------------------------------------------------------------
        QString GetConversion( double input ) const
        {
            QString output;
            Convert( input, output );
            return output;
        }
    };

    PerceptionConverter convertor;

    gui::GQ_PlotData* CreateGraphData( unsigned int hue, unsigned int valence, gui::GQ_Plot* pGraph )
    {
        gui::GQ_PlotData* plotData = new gui::GQ_PlotData( 0, *pGraph );
        QColor color;
        color.setHsv( hue, 255, valence );
        gui::GQ_PlotData::E_PointShapeType nPointShape = gui::GQ_PlotData::eDot;
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
    : Q3GroupBox( 2, Qt::Vertical, tr( "Simulation" ), parent )
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
        Q3Grid* group = new Q3Grid ( 2,this );
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
        new QLabel( tr( "Crowd Value" ), group );
        populationValue_ = new QLineEdit( group, tr( "Crowd Value" ) );
        populationValue_->setText( "0" );
        populationValue_->setValidator( new ADN_DoubleValidator( 0., static_cast< double >( std::numeric_limits< int >::max() ), 2, populationValue_ ) );
        connect( populationValue_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnPopulationChanged( const QString& ) ) );
        new QLabel( tr( "Urban Height Ratio" ), group );
        urbanHeightRatioValue_ = new QLineEdit( group, tr( "Urban Height Ratio" ) );
        urbanHeightRatioValue_->setText( "1" );
        urbanHeightRatioValue_->setValidator( new ADN_DoubleValidator( 0., 1., 2, urbanHeightRatioValue_ ) );
        connect( urbanHeightRatioValue_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUrbanHeightRatioChanged( const QString& ) ) );
        new QLabel( tr( "Urban Occupation Value" ), group );
        urbanOccupationValue_ = new QLineEdit( group, tr( "Urban Occupation Value" ) );
        urbanOccupationValue_->setText( "1" );
        urbanOccupationValue_->setValidator( new ADN_DoubleValidator( 0., 1., 2, urbanOccupationValue_ ) );
        connect( urbanOccupationValue_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUrbanOccupationChanged( const QString& ) ) );
    }

    gui::GQ_Plot* pGraph = new gui::GQ_Plot( this );
    pGraph->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    pGraph->setMinimumSize( 400, 200 );
    gui::GQ_PlotAxis& yAxis = pGraph->YAxis();
    yAxis.SetAxisCaption( tr( "Perception" ) );
    yAxis.SetAxisRange( 0, 3, true );
    yAxis.SetTicksLength( 2 );
    yAxis.ShowTicks( 2 );
    yAxis.ShowSubTicks( false );
    yAxis.ShowTicksValue( true );
    yAxis.SetValueToStringConvertor( &convertor );
    yAxis.InitTickMultiplesForIntegers();

    gui::GQ_PlotAxis& xAxis = pGraph->XAxis();
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
void ADN_Sensors_DetectionAlgorithmPrevision::EnvironmentChanged( std::string environment, double factor )
{
    environment_->setText( environment.c_str() );
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
    UpdatePreview( detection_, value, 0. );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnRecognitionChanged
// Created: HBD 2010-05-04
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnRecognitionChanged( const QString& value )
{
    UpdatePreview( recognition_, value, 0. );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnIdentificationChanged
// Created: HBD 2010-05-04
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnIdentificationChanged( const QString& value )
{
    UpdatePreview( identification_, value, 0. );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationDensityChanged
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationDensityChanged( const QString& value )
{
    UpdatePreview( populationDensityFactor_, value, 1. );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationModifierChanged
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationModifierChanged( const QString& value )
{
    UpdatePreview( populationModifier_, value, 1. );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationChanged
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnPopulationChanged( const QString& value )
{
    UpdatePreview( population_, value, 0., populationValue_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanHeightRatioChanged
// Created: LDC 2010-08-16
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanHeightRatioChanged( const QString& value )
{
    UpdatePreview( urbanHeightRatio_, value, 1., urbanHeightRatioValue_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanOccupationChanged
// Created: LDC 2010-08-16
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::OnUrbanOccupationChanged( const QString& value )
{
    UpdatePreview( urbanOccupation_, value, 1., urbanOccupationValue_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DetectionAlgorithmPrevision::UpdatePreview
// Created: LDC 2010-08-16
// -----------------------------------------------------------------------------
void ADN_Sensors_DetectionAlgorithmPrevision::UpdatePreview( double& parameter, const QString& value, double defaultValue, QLineEdit* widget /* = 0 */ )
{
    bool ok = false;
    parameter = locale().toDouble( value, &ok );
    if( !ok || value.isEmpty() )
    {
        parameter = defaultValue;
        if( widget )
            widget->setText( QString::number( defaultValue ) );
    }
    Update();
}
