// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __CheckBox_h_
#define __CheckBox_h_

namespace gui
{
// =============================================================================
/** @class  CheckBox
    @brief  Check box
*/
// Created: RPD 2008-08-21
// =============================================================================
class CheckBox : public QCheckBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CheckBox( const QString & text, QWidget * parent, const char * name = 0 );
    virtual ~CheckBox();
    //@}

    //! @name Operations
    //@{
    virtual void setValue( int id );
    virtual void setChecked( bool state );
    void Revert();
    void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CheckBox( const CheckBox& );            //!< Copy constructor
    CheckBox& operator=( const CheckBox& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int currentValue_;
    int previousValue_;
    //@}
};

}

#endif // __CheckBox_h_
