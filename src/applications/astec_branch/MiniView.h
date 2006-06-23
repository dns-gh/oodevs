// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiniView_h_
#define __MiniView_h_

class Agent;
class Positions;

// =============================================================================
/** @class  MiniView
    @brief  MiniView
*/
// Created: AGE 2006-06-23
// =============================================================================
class MiniView : public QLabel
{

public:
    //! @name Constructors/Destructor
    //@{
             MiniView( QWidget* parent, const Agent& agent );
    virtual ~MiniView();
    //@}

    //! @name Operations
    //@{
    geometry::Rectangle2f GetViewport() const;
    void SetImage( const QImage& image );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MiniView( const MiniView& );            //!< Copy constructor
    MiniView& operator=( const MiniView& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Positions& position_;
    //@}
};

#endif // __MiniView_h_
