// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlPlaceHolder_h_
#define __GlPlaceHolder_h_

namespace gui
{

// =============================================================================
/** @class  GlPlaceHolder
    @brief  GlPlaceHolder
*/
// Created: AGE 2006-05-03
// =============================================================================
class GlPlaceHolder : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GlPlaceHolder( QWidget* parent );
    virtual ~GlPlaceHolder();
    //@}
};

}

#endif // __GlPlaceHolder_h_
