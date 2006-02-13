// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamComboFiller_h_
#define __TeamComboFiller_h_

class Team;

// =============================================================================
/** @class  TeamComboFiller
    @brief  Team combo filler
*/
// Created: AGE 2006-02-13
// =============================================================================
template< typename Combo, typename Method >
class TeamComboFiller
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamComboFiller( Combo* combo, Method m ) : combo_( combo ), m_( m ) {};
    virtual ~TeamComboFiller();
    //@}

    //! @name Operations
    //@{
    void operator()( const Team& team ) const
    {
        combo_->AddItem( team.GetName().c_str(), team.*m_() );
    };
    //@}

private:
    //! @name Member data
    //@{
    Combo* combo_;
    Method m_;
    //@}
};

template< typename Combo, typename Method >
TeamComboFiller< Combo, typename Method > TeamComboFill( Combo* combo, Method m )
{
    return TeamComboFiller< Combo >( combo, m );
}


#endif // __TeamComboFiller_h_
