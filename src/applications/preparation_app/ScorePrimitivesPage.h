// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScorePrimitivesPage_h_
#define __ScorePrimitivesPage_h_

#include "tools/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"
#include <boost/function.hpp>

namespace kernel
{
    class ModelLoaded;
    class ModelUnLoaded;
    class Controllers;
}

namespace indicators
{
    class Primitive;
    class Primitives;
}

// =============================================================================
/** @class  ScorePrimitivesPage
    @brief  ScorePrimitivesPage
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScorePrimitivesPage : public Q3VBox
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                          , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT;

public:
    //! @name Types
    //@{
    typedef boost::function< bool( const indicators::Primitive& ) > T_Filter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ScorePrimitivesPage( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const indicators::Primitives& primitives, const T_Filter& filter );
    virtual ~ScorePrimitivesPage();
    //@}

    //! @name Operations
    //@{
    void Display( const indicators::Primitive& primitive, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

signals:
    //! @name Signals
    //@{
    void Insert( const QString& text );
    void Selected( const indicators::Primitive& primitive );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnInsert();
    void OnSelectionChanged();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScorePrimitivesPage( const ScorePrimitivesPage& );            //!< Copy constructor
    ScorePrimitivesPage& operator=( const ScorePrimitivesPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Filter filter_;
    const indicators::Primitives& primitives_;
    gui::ListDisplayer< ScorePrimitivesPage >* list_;
    //@}
};

#endif // __ScorePrimitivesPage_h_
