// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichDialogButtonBox_h_
#define __RichDialogButtonBox_h_

namespace gui
{

// =============================================================================
/** @class  RichDialogButtonBox
    @brief  RichDialogButtonBox
*/
// Created: NPT 2013-03-22
// =============================================================================
class RichDialogButtonBox : public QDialogButtonBox
{

public:
    //! @name Constructors/Destructor
    //@{
             RichDialogButtonBox( const QString& objectname, StandardButtons buttonType );
    virtual ~RichDialogButtonBox();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __RichDialogButtonBox_h_
