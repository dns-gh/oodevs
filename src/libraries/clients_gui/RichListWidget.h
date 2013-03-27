// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichListWidget_h_
#define __RichListWidget_h_

namespace gui
{
// =============================================================================
/** @class  RichListWidget
    @brief  RichListWidget
*/
// Created: NPT 2013-03-18
// =============================================================================
class RichListWidget : public QListWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             RichListWidget( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichListWidget();
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

#endif // __RichListWidget_h_
