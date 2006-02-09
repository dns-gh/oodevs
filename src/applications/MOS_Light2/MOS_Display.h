// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Display_h_
#define __MOS_Display_h_

class MOS_DisplayGroup;

// =============================================================================
/** @class  MOS_Display
    @brief  MOS_Display
*/
// Created: AGE 2006-02-09
// =============================================================================
class MOS_Display
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Display( QWidget* parent );
    virtual ~MOS_Display();
    //@}

    //! @name Operations
    //@{
    MOS_DisplayGroup& AddGroup( const char* name );
    MOS_DisplayGroup& Group( const char* name );
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
    MOS_Display( const MOS_Display& );            //!< Copy constructor
    MOS_Display& operator=( const MOS_Display& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, MOS_DisplayGroup* > T_Groups;
    typedef T_Groups::iterator                        IT_Groups;
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    T_Groups groups_;
    //@}
};

#endif // __MOS_Display_h_
