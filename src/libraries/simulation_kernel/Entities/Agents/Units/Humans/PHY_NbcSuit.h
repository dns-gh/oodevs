// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PHY_NbcSuit_h
#define PHY_NbcSuit_h

#include "MT_Tools/MT_String.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

// =============================================================================
/** @class  PHY_NbcSuit
    @brief  PHY_NbcSuit
*/
// Created: LGY 2012-11-26
// =============================================================================
class PHY_NbcSuit : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef boost::shared_ptr< const PHY_NbcSuit >                   T_NbcSuit;
    typedef std::map< std::string, T_NbcSuit, sCaseInsensitiveLess > T_NbcSuitMap;
    typedef T_NbcSuitMap::const_iterator                           CIT_NbcSuitMap;

    enum E_AgentNbcSuit
    {
        eAgentNone,
        eAgentNbcSuitLevel1,
        eAgentNbcSuitLevel2,
        eAgentNbcSuitLevel3,
        eAgentNbcSuitLevel4,
        eAgentNbcSuitLevel5
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_NbcSuit( const std::string& strName, E_AgentNbcSuit suit );
    virtual ~PHY_NbcSuit();
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static boost::shared_ptr< const PHY_NbcSuit > Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetType() const;
    //@}

private:
    //! @name Member data
    //@{
    static T_NbcSuitMap nbcSuits_;
    const std::string strName_;
    E_AgentNbcSuit suit_;
    //@}
};

#endif // PHY_NbcSuit_h
