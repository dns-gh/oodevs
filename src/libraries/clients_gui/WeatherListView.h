// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_WeatherListView_h_
#define __gui_WeatherListView_h_

#include <boost/noncopyable.hpp>

namespace boost
{
    template<typename T> class shared_ptr;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace weather
{
    class Meteo;
}

namespace gui
{
// =============================================================================
/** @class  WeatherListView
    @brief  Weather list view
*/
// Created: ABR 2011-05-30
// =============================================================================
class WeatherListView : public QTreeView
                      , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< weather::Meteo > > T_Weathers;
    typedef T_Weathers::iterator                              IT_Weathers;
    typedef T_Weathers::const_iterator                       CIT_Weathers;
    //@}

    //! @name Constructors/Destructor
    //@{
             WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter );
    virtual ~WeatherListView();
    //@}

    //! @name Operations
    //@{
    weather::Meteo* SelectedItem();
    const T_Weathers& GetLocalWeathers() const;
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void CreateItem() = 0;
    virtual void DeleteItem();
    //@}

private:
    //! @name Operations
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

protected:
    //! @name Helpers
    //@{
    void Clear();
    //@}

protected:
    //! @name Member data
    //@{
    QStandardItemModel*                    model_;
    const kernel::CoordinateConverter_ABC& converter_;
    T_Weathers                             weathers_;
    //@}

};

}

#endif // __gui_WeatherListView_h_
