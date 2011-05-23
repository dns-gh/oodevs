// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_LogisticLevel.h $
// $Author: Jvt $
// $Modtime: 9/12/04 15:09 $
// $Revision: 1 $
// $Workfile: PHY_LogisticLevel.h $
//
// *****************************************************************************

#ifndef __PHY_LogisticLevel_h_
#define __PHY_LogisticLevel_h_

#include "MT_Tools/MT_String.h"
#include <boost/noncopyable.hpp>
#include <map>

// =============================================================================
// @class  PHY_LogisticLevel
// Created: JVT 2004-08-03
// =============================================================================
class PHY_LogisticLevel : private boost::noncopyable
{

public:
    //! @name Manager
    //@{
    static PHY_LogisticLevel none_;
    static PHY_LogisticLevel logistic_base_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_LogisticLevel* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&            GetName () const;
    int                           GetID() const;
    //@}

    //! @name Operators
    //@{
    bool operator== ( const PHY_LogisticLevel& rhs ) const;
    bool operator!= ( const PHY_LogisticLevel& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_LogisticLevel* > T_LogisticLevelMap;
    typedef T_LogisticLevelMap::const_iterator              CIT_LogisticLevelMap;
    //@}

private:
     PHY_LogisticLevel( const std::string& strName, int id);
    ~PHY_LogisticLevel();

private:
    const std::string strName_;
    int               id_;

private:
    static T_LogisticLevelMap logisticLevels_;
};

#endif // __PHY_LogisticLevel_h_

