// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Sensors_DetectionAlgorithmPrevision_h_
#define __ADN_Sensors_DetectionAlgorithmPrevision_h_

#include "ENT/ENT_Enums_Gen.h"
#include "ADN_Connector_ABC.h"
#include "ADN_Sensors_Data.h"

class ModificatorEnvironmentInfos;
class ADN_GuiBuilder;
class GQ_PlotData;

// =============================================================================
/** @class  ADN_Sensors_DetectionAlgorithmPrevision
    @brief  ADN_Sensors_DetectionAlgorithmPrevision
*/
// Created: HBD 2010-04-26
// =============================================================================
class ADN_Sensors_DetectionAlgorithmPrevision : public Q3GroupBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Sensors_DetectionAlgorithmPrevision( QWidget* parent );
    virtual ~ADN_Sensors_DetectionAlgorithmPrevision();
    //@}

public slots:
    //! @name Operations
    //@{
    void OnSelectSensor( void* data );
    void OnPerceiverStanceChanged( std::string posture, double factor );
    void OnTargetStanceChanged( std::string targetPosture, double factor );
    void OnSizeChanged( std::string volume, double factor );
    void WeatherChanged( std::string weather, double factor );
    void IlluminationChanged( std::string illumination, double factor );
    void EnvironmentChanged( ADN_Sensors_Data::ModificatorEnvironmentInfos* env, double factor );
    void UrbanBlockChanged( std::string urban, double factor );
    void OnDetectionChanged( const QString& );
    void OnRecognitionChanged( const QString& );
    void OnIdentificationChanged( const QString& );
    void OnPopulationDensityChanged( const QString& );
    void OnPopulationModifierChanged( const QString& );
    void OnPopulationChanged( const QString& );
    void OnUrbanHeightRatioChanged( const QString& );
    void OnUrbanOccupationChanged( const QString& );
    void UpdateValue();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_Sensors_DetectionAlgorithmPrevision( const ADN_Sensors_DetectionAlgorithmPrevision& );            //!< Copy constructor
    ADN_Sensors_DetectionAlgorithmPrevision& operator=( const ADN_Sensors_DetectionAlgorithmPrevision& ); //!< Assignment operator
    //@}

    //! @name Modifiers
    //@{
    void Update();
    //@}
    //! @name Helpers
    //@{
    void UpdatePreview( double& parameter, QLineEdit* widget, double defaultValue, const QString& defaultString, const QString& value );
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* volume_;
    QLineEdit* stance_;
    QLineEdit* stanceTarget_;
    QLineEdit* weather_;
    QLineEdit* illumnination;
    QLineEdit* environment_;
    QLineEdit* urbanMaterial_;
    QLineEdit* populationValue_;
    QLineEdit* urbanHeightRatioValue_;
    QLineEdit* urbanOccupationValue_;
    GQ_PlotData* pGraphData_;
    GQ_PlotData* pBaseGraphData_;
    double perceiverPostureFactor_;
    double targetPostureFactor_;
    double sizeFactor_;
    double weatherFactor_;
    double illuminationFactor_;
    double environmentFactor_;
    double populationDensityFactor_;
    double population_;
    double urbanHeightRatio_;
    double urbanOccupation_;
    double detection_;
    double recognition_;
    double identification_;
    double populationModifier_;
    bool urbanSelected_;
    //@}
};

#endif // __ADN_Sensors_DetectionAlgorithmPrevision_h_
