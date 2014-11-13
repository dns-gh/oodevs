// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencePanel_ABC_h_
#define __PreferencePanel_ABC_h_

#pragma warning( push, 0 )
#include <QtGui/qscrollarea.h>
#pragma warning( pop )

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  PreferencePanel_ABC
    @brief  PreferencePanel_ABC
*/
// Created: SBO 2007-01-03
// =============================================================================
class PreferencePanel_ABC : public QScrollArea
{
public:
    //! @name Constructors/Destructor
    //@{
             PreferencePanel_ABC( QWidget* parent, const QString& objectName );
    virtual ~PreferencePanel_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const GLView_ABC& );
    //@}
};

} //! namespace gui

#endif // __PreferencePanel_ABC_h_
