// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlobalWeathersList_h_
#define __GlobalWeathersList_h_

namespace kernel
{
    class CoordinateConverter_ABC;
}

class WeatherModel;
class Weather;

// =============================================================================
/** @class  LocalWeathersList
@brief  Local weathers list
*/
// Created: SBO 2006-12-20
// =============================================================================
class GlobalWeathersList : public QListView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    GlobalWeathersList( QWidget* parent, const kernel::CoordinateConverter_ABC& converter );
    virtual ~GlobalWeathersList();
    //@}

    //! @name Operations
    //@{
    void CommitTo( WeatherModel& model );
    void Update( const WeatherModel& model );
    Weather* SelectedItem();
    //@}

    private slots:
        //! @name Slots
        //@{
        void ContextMenuRequested( QListViewItem* item, const QPoint& point, int column );
        void CreateItem();
        void DeleteItem();
        //@}

private:
    //! @name Copy/Assignement
    //@{
    GlobalWeathersList( const GlobalWeathersList& );            //!< Copy constructor
    GlobalWeathersList& operator=( const GlobalWeathersList& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Clear();
    //@}

    //! @name Types
    //@{
    typedef std::vector< Weather* > T_Weathers;
    typedef T_Weathers::iterator   IT_Weathers;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_Weathers      weathers_;
    //@}
};

#endif // __LocalWeathersList_h_
