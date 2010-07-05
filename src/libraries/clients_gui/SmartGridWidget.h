// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SmartGridWidget_h_
#define __SmartGridWidget_h_

namespace gui
{

// =============================================================================
/** @class  SmartGridWidget
    @brief  SmartGridWidget
*/
// Created: AGE 2006-06-23
// =============================================================================
class SmartGridWidget : public QWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             SmartGridWidget( QWidget* parent, int strips, Orientation o, const char* name = 0 );
    virtual ~SmartGridWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SmartGridWidget( const SmartGridWidget& );            //!< Copy constructor
    SmartGridWidget& operator=( const SmartGridWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void childEvent( QChildEvent * );
    void AddChild( QWidget* child );
    void Reorganize();
    //@}

private:
    //! @name Member data
    //@{
    QGridLayout* layout_;
    int strips_;
    Orientation o_;
    unsigned int childCount_;
    //@}
};

}

#endif // __SmartGridWidget_h_
