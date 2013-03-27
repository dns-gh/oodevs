// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichTableWidget_h_
#define __RichTableWidget_h_

namespace gui
{
// =============================================================================
/** @class  RichTableWidget
    @brief  RichTableWidget
*/
// Created: NPT 2013-03-18
// =============================================================================
class RichTableWidget : public QTableWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             RichTableWidget( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichTableWidget();
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

#endif // __RichTableWidget_h_
