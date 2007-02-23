// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectCreationPanel_h_
#define __ObjectCreationPanel_h_

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class ParametersLayer;
}

class ObjectPrototype;
class StaticModel;
class Publisher_ABC;

// =============================================================================
/** @class  ObjectCreationPanel
    @brief  Object creation panel
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectCreationPanel : public QVBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectCreationPanel( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& model, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools );
    virtual ~ObjectCreationPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( const kernel::Viewport_ABC& viewport );
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectCreationPanel( const ObjectCreationPanel& );            //!< Copy constructor
    ObjectCreationPanel& operator=( const ObjectCreationPanel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    const kernel::GlTools_ABC& tools_;
    ObjectPrototype* created_;
    //@}
};

#endif // __ObjectCreationPanel_h_
