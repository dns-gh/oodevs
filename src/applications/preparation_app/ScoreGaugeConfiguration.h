// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreGaugeConfiguration_h_
#define __ScoreGaugeConfiguration_h_

#include "clients_gui/ValuedComboBox.h"
#include <tools/ElementObserver_ABC.h>

namespace indicators
{
    class Gauge;
    class GaugeType;
    class GaugeTypes;
}

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class RichSpinBox;
    class RichLineEdit;
    class RichGroupBox;
    template< typename T > class RichWidget;
}

// =============================================================================
/** @class  ScoreGaugeConfiguration
    @brief  ScoreGaugeConfiguration
*/
// Created: SBO 2009-05-05
// =============================================================================
class ScoreGaugeConfiguration : public Q3VBox
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                              , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ScoreGaugeConfiguration( QWidget* parent, kernel::Controllers& controllers, const indicators::GaugeTypes& gaugeTypes );
    virtual ~ScoreGaugeConfiguration();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const indicators::Gauge& gauge );
    indicators::Gauge GetValue() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeStep( int steps );
    void OnChangeBoundaries();
    void OnTypeChanged();
    void OnChangeValue( int row, int col );
    void OnReverseSymbols();
    //@}

private:
    //! @name Helpers
    //@{
    void AddInterval( double min = 0, double max = 0, double key = 0 );
    void RemoveInterval();
    void UpdateSymbol( int row, double value );
    void SetValue( int row, int col, double value );
    double GetValue( int row, int col ) const;

    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const indicators::GaugeTypes& gaugeTypes_;
    gui::ValuedComboBox< const indicators::GaugeType* >* type_;
    gui::RichGroupBox* normalizationBox_;
    gui::RichSpinBox* steps_;
    gui::RichLineEdit* min_;
    gui::RichLineEdit* max_;
    gui::RichWidget< QTableWidget >* intervals_;
    //@}
};

#endif // __ScoreGaugeConfiguration_h_
