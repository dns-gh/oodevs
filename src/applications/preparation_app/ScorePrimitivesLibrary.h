// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScorePrimitivesLibrary_h_
#define __ScorePrimitivesLibrary_h_

#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

class IndicatorPrimitive;

// =============================================================================
/** @class  ScorePrimitivesLibrary
    @brief  ScorePrimitivesLibrary
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScorePrimitivesLibrary : public QTabWidget
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScorePrimitivesLibrary( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Resolver_ABC< IndicatorPrimitive, QString >& primitives );
    virtual ~ScorePrimitivesLibrary();
    //@}

signals:
    //! @name Slots
    //@{
    void Insert( const QString& text );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScorePrimitivesLibrary( const ScorePrimitivesLibrary& );            //!< Copy constructor
    ScorePrimitivesLibrary& operator=( const ScorePrimitivesLibrary& ); //!< Assignment operator
    //@}
};

#endif // __ScorePrimitivesLibrary_h_
