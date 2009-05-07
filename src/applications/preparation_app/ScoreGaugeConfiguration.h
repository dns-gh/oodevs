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

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include <qvgroupbox.h>

namespace indicators
{
    class GaugeType;
    class GaugeTypes;
}

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

// =============================================================================
/** @class  ScoreGaugeConfiguration
    @brief  ScoreGaugeConfiguration
*/
// Created: SBO 2009-05-05
// =============================================================================
class ScoreGaugeConfiguration : public QVGroupBox
                              , public kernel::Observer_ABC
                              , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                              , public kernel::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreGaugeConfiguration( QWidget* parent, kernel::Controllers& controllers, const indicators::GaugeTypes& gaugeTypes );
    virtual ~ScoreGaugeConfiguration();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeStep( int steps );
    void OnChangeBoundaries();
    void OnTypeChanged();
    void OnChangeValue( int row, int col );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreGaugeConfiguration( const ScoreGaugeConfiguration& );            //!< Copy constructor
    ScoreGaugeConfiguration& operator=( const ScoreGaugeConfiguration& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddInterval();
    void RemoveInterval();
    void UpdateSymbol( int row, double value );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const indicators::GaugeTypes& gaugeTypes_;
    gui::ValuedComboBox< const indicators::GaugeType* >* type_;
    QGroupBox* normalizationBox_;
    QLineEdit* min_;
    QLineEdit* max_;
    QTable* intervals_;
    //@}
};

#endif // __ScoreGaugeConfiguration_h_
