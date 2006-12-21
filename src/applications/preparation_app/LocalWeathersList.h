// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocalWeathersList_h_
#define __LocalWeathersList_h_

namespace kernel
{
    class CoordinateConverter_ABC;
}

class WeatherModel;
class LocalWeather;

// =============================================================================
/** @class  LocalWeathersList
    @brief  Local weathers list
*/
// Created: SBO 2006-12-20
// =============================================================================
class LocalWeathersList : public QListView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocalWeathersList( QWidget* parent, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocalWeathersList();
    //@}

    //! @name Operations
    //@{
    void CommitTo( WeatherModel& model );
    void Update( const WeatherModel& model );
    LocalWeather* SelectedItem();
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
    LocalWeathersList( const LocalWeathersList& );            //!< Copy constructor
    LocalWeathersList& operator=( const LocalWeathersList& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Clear();
    //@}

    //! @name Types
    //@{
    typedef std::vector< LocalWeather* > T_LocalWeathers;
    typedef T_LocalWeathers::iterator   IT_LocalWeathers;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_LocalWeathers      weathers_;
    //@}
};

#endif // __LocalWeathersList_h_
