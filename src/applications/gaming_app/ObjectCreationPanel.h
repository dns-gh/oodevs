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

class ObjectPrototype;
class Controllers;
class StaticModel;
class ParametersLayer;
class GlTools_ABC;
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
             ObjectCreationPanel( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const StaticModel& model, ParametersLayer& layer, const GlTools_ABC& tools );
    virtual ~ObjectCreationPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( const geometry::Rectangle2f& viewport );
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
    const GlTools_ABC& tools_;
    ObjectPrototype* created_;
    //@}
};

#endif // __ObjectCreationPanel_h_
