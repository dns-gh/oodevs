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

#include "clients_gui/RichWidget.h"

namespace kernel
{
    class Controllers;
}

namespace indicators
{
    class Primitive;
    class Primitives;
}

class ScorePrimitivesPage;

// =============================================================================
/** @class  ScorePrimitivesLibrary
    @brief  ScorePrimitivesLibrary
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScorePrimitivesLibrary : public gui::RichWidget< QTabWidget >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ScorePrimitivesLibrary( QWidget* parent, kernel::Controllers& controllers, const indicators::Primitives& primitives );
    virtual ~ScorePrimitivesLibrary();
    //@}

signals:
    //! @name Slots
    //@{
    void Insert( const QString& text );
    void Selected( const indicators::Primitive& indicator );
    //@}

private:
    //! @name Helpers
    //@{
    void AddPage( const QString& name, QWidget* page );
    //@}
};

#endif // __ScorePrimitivesLibrary_h_
