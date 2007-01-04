// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StatisticsWidget_h_
#define __StatisticsWidget_h_

// =============================================================================
/** @class  StatisticsWidget
    @brief  StatisticsWidget
*/
// Created: SBO 2007-01-04
// =============================================================================
class StatisticsWidget : public QListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit StatisticsWidget( QWidget* parent );
    virtual ~StatisticsWidget();
    //@}

    //! @name Operations
    //@{
    void AddValue( unsigned long value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StatisticsWidget( const StatisticsWidget& );            //!< Copy constructor
    StatisticsWidget& operator=( const StatisticsWidget& ); //!< Assignement operator
    //@}
};

#endif // __StatisticsWidget_h_
