// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __StatisticsWidget_h_
#define __StatisticsWidget_h_

#include "clients_gui/GQ_Plot.h"

namespace gui
{
    class GQ_PlotData;
}

// =============================================================================
/** @class  StatisticsWidget
    @brief  StatisticsWidget
*/
// Created: SBO 2007-01-04
// =============================================================================
class StatisticsWidget : public gui::GQ_Plot
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit StatisticsWidget( QWidget* parent );
    virtual ~StatisticsWidget();
    //@}

    //! @name Operations
    //@{
    void AddValue( unsigned int tick, unsigned long value );
    void SetYAxisCaption( const QString& caption );
    void SetXAxisCaption( const QString& caption );
    void SetVisibleTicks( unsigned int count );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StatisticsWidget( const StatisticsWidget& );            //!< Copy constructor
    StatisticsWidget& operator=( const StatisticsWidget& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    gui::GQ_PlotData* data_;
    unsigned long yMax_;
    T_Values lastValues_;
    unsigned int visibleTicks_;
    //@}
};

#endif // __StatisticsWidget_h_
