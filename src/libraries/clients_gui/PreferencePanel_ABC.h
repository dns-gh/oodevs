// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencePanel_ABC_h_
#define __PreferencePanel_ABC_h_

#pragma warning( push, 0 )
#include <QtGui/qscrollarea.h>
#pragma warning( pop )

namespace gui
{
    class GlProxy;

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
    explicit PreferencePanel_ABC( QWidget* parent, const char* name = 0 );
    virtual ~PreferencePanel_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const GlProxy& ) {}
    virtual void Commit() {}
    virtual void Reset() {}
    //@}
};

}

#endif // __PreferencePanel_ABC_h_
