// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Graphics3dPanel_h_
#define __Graphics3dPanel_h_

#include "PreferencePanel_ABC.h"

namespace gui
{

// =============================================================================
/** @class  Graphics3dPanel
    @brief  Graphics3dPanel
*/
// Created: SBO 2007-01-03
// =============================================================================
class Graphics3dPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit Graphics3dPanel( QWidget* parent );
    virtual ~Graphics3dPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void TypeChanged( int );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Graphics3dPanel( const Graphics3dPanel& );            //!< Copy constructor
    Graphics3dPanel& operator=( const Graphics3dPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QButtonGroup* type_;
    QGroupBox* elevationBox_;
    //@}
};

}

#endif // __Graphics3dPanel_h_
