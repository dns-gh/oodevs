// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoStancesWidget_h_
#define __InfoStancesWidget_h_

#include "tools/SelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Attributes_ABC;
}

class Attributes;

// =============================================================================
/** @class  InfoStancesWidget
    @brief  InfoStancesWidget
*/
// Created: SBO 2007-02-09
// =============================================================================
class InfoStancesWidget : public Q3VBox
                        , public tools::Observer_ABC
                        , public tools::SelectionObserver< kernel::Entity_ABC >
                        , public tools::ElementObserver_ABC< kernel::Attributes_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoStancesWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~InfoStancesWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoStancesWidget( const InfoStancesWidget& );            //!< Copy constructor
    InfoStancesWidget& operator=( const InfoStancesWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Attributes_ABC& extension );
    void Update( const Attributes& attributes );
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    Q3ProgressBar* movement_;
    Q3ProgressBar* installation_;
    //@}
};

#endif // __InfoStancesWidget_h_
