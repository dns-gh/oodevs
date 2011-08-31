 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Humans_h_
#define __Humans_h_

namespace sword
{
    enum EnumHumanRank;
    enum EnumHumanState;
    enum EnumHumanLocation;
    enum EnumInjuriesSeriousness;
    class HumanDotations_HumanDotation;
}

namespace dispatcher
{

// =============================================================================
/** @class  Humans
    @brief  Humans
*/
// Created: NLD 2006-09-19
// =============================================================================
class Humans
{
public:
    //! @name Constructors/Destructor
    //@{
             Humans();
    virtual ~Humans();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::HumanDotations_HumanDotation& asnMsg );
    void Send  ( sword::HumanDotations_HumanDotation& asnMsg ) const ;
    //@}

public:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, sword::EnumInjuriesSeriousness >  T_Injury;

    typedef std::vector< T_Injury >                                    T_Injuries;
    typedef T_Injuries::iterator                                      IT_Injuries;
    typedef T_Injuries::const_iterator                               CIT_Injuries;
    //@}

public:
    //! @name Data members
    //@{
    unsigned int             number_;
    sword::EnumHumanRank     rank_;
    sword::EnumHumanState    state_;
    sword::EnumHumanLocation location_;
    T_Injuries               injuries_;
    bool                     contaminated_;
    bool                     psyop_;
    //@}
};

}

#endif // __Humans_h_
