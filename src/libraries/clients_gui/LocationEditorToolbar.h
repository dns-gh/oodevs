// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationEditorToolbar_h_
#define __LocationEditorToolbar_h_

#include "LocationEditor_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
}

namespace gui
{
    class ParametersLayer;
    class View_ABC;

// =============================================================================
/** @class  LocationEditorToolbar
    @brief  LocationEditorToolbar
*/
// Created: SBO 2007-03-06
// =============================================================================
class LocationEditorToolbar : public QToolBar
                            , public LocationEditor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view );
    virtual ~LocationEditorToolbar();
    //@}

    //! @name Operations
    //@{
    virtual void StartEdit( ParametersLayer& parameters );
    virtual void EndEdit();
    //@}

private slots:
    //! @name Slots
    //@{
    void Goto();
    void AddPoint();
    void AddParamPoint();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationEditorToolbar( const LocationEditorToolbar& );            //!< Copy constructor
    LocationEditorToolbar& operator=( const LocationEditorToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    geometry::Point2f GetPosition() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    View_ABC& view_;
    ParametersLayer* parameters_;
    QLineEdit* utm_;
    QToolButton* okButton_;
    QToolButton* paramsButton_;
    QToolButton* gotoButton_;
    //@}
};

}

#endif // __LocationEditorToolbar_h_
