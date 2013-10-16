// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseMenu_h_
#define __ExerciseMenu_h_

#include "clients_gui/RichMenu.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class ExerciseConfig;
}

// =============================================================================
/** @class  ExerciseMenu
    @brief  Exercise menu to access specific resources
    // $$$$ SBO 2008-08-27: Evolution: resources should probably depend on the profile used
*/
// Created: SBO 2008-08-27
// =============================================================================
class ExerciseMenu : public gui::RichMenu
                   , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                   , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseMenu( QWidget* parent, kernel::Controllers& controllers, const QString& title = "" );
    virtual ~ExerciseMenu();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnSelect( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& model );

    void Reset();
    void ReadResource( xml::xistream& xis, const tools::ExerciseConfig& config );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    tools::Path::T_Paths links_;
    //@}
};

#endif // __ExerciseMenu_h_
