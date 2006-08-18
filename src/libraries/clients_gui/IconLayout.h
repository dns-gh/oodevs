// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IconLayout_h_
#define __IconLayout_h_

// =============================================================================
/** @class  IconLayout
    @brief  IconLayout
*/
// Created: SBO 2006-08-18
// =============================================================================
class IconLayout
{

public:
    //! @name Constructors/Destructor
    //@{
             IconLayout();
    virtual ~IconLayout();
    //@}

    //! @name Operations
    //@{
    void AddIcon( const char** xpm, int x, int y );
    const geometry::Point2f& IconLocation( const char** xpm );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    IconLayout( const IconLayout& );            //!< Copy constructor
    IconLayout& operator=( const IconLayout& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const char**, geometry::Point2f > T_IconLocations;
    //@}

private:
    //! @name Member data
    //@{
    T_IconLocations iconLocations_;
    //@}
};

#endif // __IconLayout_h_
