// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineItem_ABC_h_
#define __TimelineItem_ABC_h_


// =============================================================================
/** @class  TimelineItem_ABC
    @brief  TimelineItem_ABC
*/
// Created: SBO 2007-07-06
// =============================================================================
class TimelineItem_ABC : public Q3CanvasRectangle
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TimelineItem_ABC( Q3Canvas* canvas );
    virtual ~TimelineItem_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Update() = 0;
    virtual void Move( long offset );
    virtual void Delete();
    virtual void DisplayToolTip( QWidget* parent ) const; // $$$$ SBO 2007-07-19: make a displayer maybe...
    virtual void DisplayContextMenu( Q3PopupMenu* menu ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineItem_ABC( const TimelineItem_ABC& );            //!< Copy constructor
    TimelineItem_ABC& operator=( const TimelineItem_ABC& ); //!< Assignment operator
    //@}
};

#endif // __TimelineItem_ABC_h_
