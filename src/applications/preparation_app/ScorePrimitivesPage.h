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

#include <tools/ElementObserver_ABC.h>

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

namespace gui
{
    template< typename T > class RichWidget;
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
    typedef std::function< bool( const indicators::Primitive& ) > T_Filter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ScorePrimitivesPage( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const indicators::Primitives& primitives, const T_Filter& filter );
    virtual ~ScorePrimitivesPage();
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
    gui::RichWidget< QTreeWidget >* list_;
    //@}
};

#endif // __ScorePrimitivesPage_h_
