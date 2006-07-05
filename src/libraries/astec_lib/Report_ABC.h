// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Report_ABC.h $
// $Author: Ape $
// $Modtime: 4/08/04 15:18 $
// $Revision: 2 $
// $Workfile: Report_ABC.h $
//
// *****************************************************************************

#ifndef __Report_ABC_h_
#define __Report_ABC_h_

class Agent_ABC;
class Simulation;
class Displayer_ABC;

// =============================================================================
/** @class  Report_ABC
    @brief  Report_ABC
    // $$$$ AGE 2006-06-29: Refactor !
    // $$$$ AGE 2006-07-03: C'est _vraiment_ � chier
*/
// Created: APE 2004-08-04
// =============================================================================
class Report_ABC
{
public:
    enum E_Type
    {
        eRC      = 1000,
        eTrace   = 1001,
        eEvent   = 1002,
        eMessage = 1003,
        eWarning = 1004
    };

public:
    //! @name Constructors/Destructor
    //@{
             Report_ABC( const Agent_ABC& agent, const Simulation& simulation );
    virtual ~Report_ABC();
    //@}

    //! @name Operations
    //@{
    void DisplayInTooltip( Displayer_ABC& displayer ) const;
    virtual void Display( Displayer_ABC& displayer ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const Agent_ABC& GetAgent() const;
    
    E_Type      GetType () const;
    bool        IsRCType() const;

    bool IsNew() const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Report_ABC( const Report_ABC& );
    Report_ABC& operator=( const Report_ABC& );
    //@}

    //! @name Helpers
    //@{
    std::string GetStrippedTitle() const;
    static QColor GetColor( E_Type type );
    //@}

protected:
    //! @name Member data
    //@{
    const Agent_ABC& agent_;
    std::string strTitle_; 
    int         nTime_;
    bool        bNew_;
    E_Type      eType_;
    //@}
};

#endif // __Report_ABC_h_
