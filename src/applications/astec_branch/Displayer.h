// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Display_h_
#define __Display_h_

class DisplayGroup;

// =============================================================================
/** @class  Displayer
    @brief  Displayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class Displayer
{

public:
    //! @name Constructors/Destructor
    //@{
             Displayer( QWidget* parent );
    virtual ~Displayer();
    //@}

    //! @name Operations
    //@{
    DisplayGroup& AddGroup( const char* name );
    DisplayGroup& Group( const char* name );
    void Clear();
    //@}

    //! @name Helpers
    //@{
    static QString YesNo( bool );
    static QString Id( unsigned int );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Displayer( const Displayer& );            //!< Copy constructor
    Displayer& operator=( const Displayer& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, DisplayGroup* > T_Groups;
    typedef T_Groups::iterator                        IT_Groups;
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    T_Groups groups_;
    //@}
};

#endif // __Display_h_
