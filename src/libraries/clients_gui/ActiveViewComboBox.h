// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ActiveViewComboBox_h
#define ActiveViewComboBox_h

namespace gui
{
    class GLMainProxy;

// =============================================================================
/** @class  ActiveViewComboBox
    @brief  ActiveViewComboBox
*/
// Created: ABR 2014-07-23
// =============================================================================
class ActiveViewComboBox : public QComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
             ActiveViewComboBox( GLMainProxy& mainProxy,
                                 const QString& objectName,
                                 QWidget* parent = 0 );
    virtual ~ActiveViewComboBox();
    //@}
};

} //! namespace gui

#endif // ActiveViewComboBox_h
