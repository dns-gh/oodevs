// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TristateOption_h_
#define __TristateOption_h_

template< typename T > class ToolListButton;

// =============================================================================
/** @class  TristateOption
    @brief  TristateOption
*/
// Created: AGE 2006-03-30
// =============================================================================
class TristateOption
{
public:
    //! @name Constants
    //@{
    static const TristateOption off_;
    static const TristateOption on_;
    static const TristateOption auto_;
    //@}

    //! @name Helpers
    //@{
    static void Populate( ToolListButton< TristateOption >& button );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit TristateOption( const std::string& state = "Off" );
            ~TristateOption();
    //@}

    //! @name Operations
    //@{
    bool IsSet( bool automaticCondition ) const;
    bool operator==( const TristateOption& rhs ) const { return rhs.state_ == state_; };
    operator const char*() const;
    //@}

private:
    //! @name Member data
    //@{
    char state_;
    //@}
};

#endif // __TristateOption_h_
