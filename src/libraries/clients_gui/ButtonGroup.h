// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ButtonGroup_h_
#define __ButtonGroup_h_

namespace gui
{

// =============================================================================
/** @class  ButtonGroup
    @brief  ButtonGroup
*/
// Created: RPD 2008-08-21
// =============================================================================
class ButtonGroup : public Q3ButtonGroup
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ButtonGroup( int columns, Qt::Orientation o, const QString& title, QWidget* parent = 0, const char* name = 0 );
    virtual ~ButtonGroup();
    //@}

    //! @name Operations
    //@{
    virtual void setButton( int id );
    void Revert();
    void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ButtonGroup( const ButtonGroup& );            //!< Copy constructor
    ButtonGroup& operator=( const ButtonGroup& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int currentId_;
    int previousId_;
    //@}
};

}

#endif // __ButtonGroup_h_
