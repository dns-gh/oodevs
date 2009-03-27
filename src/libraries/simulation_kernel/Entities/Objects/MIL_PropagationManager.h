// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//------------------------------------------------------------------------------
// Name: MIL_DynamicFire.cpp
// Created: RFT 28/04/2008
// Modified: none
//------------------------------------------------------------------------------

#ifndef __MIL_PropagationManager_h_
#define __MIL_PropagationManager_h_

#include <set>
//#include <map>

// =============================================================================
/** @class  MIL_PropagationManager
    @brief  MIL_PropagationManager    
*/
// Created: RFT 28/04/2008
// Modified: RFT 05/05/2008
// =============================================================================
class MIL_PropagationManager
{
    MT_COPYNOTALLOWED( MIL_PropagationManager );

public:
    //! @name Constructors/Destructor
    //@{
            MIL_PropagationManager();
    virtual ~MIL_PropagationManager();
    //@}

	//! @name Methods
    //@{
    void    Flag( const MT_Vector2D& vOrigin );
    void    RemoveFlag( const MT_Vector2D& vOrigin );
    bool    IsFlagged( const TER_Localisation& localisation ) const;
	//@}

private:
    typedef std::pair< int, int > T_Coordonnees;
    //typedef std::map< T_Coordonnees, MIL_DynamicFire* > T_DynamicFireMap;
    typedef std::set< T_Coordonnees > T_DynamicFireSet;

private:
    //T_DynamicFireMap dynamicFireMap_;
    T_DynamicFireSet dynamicFireSet_;
};

#endif // __MIL_PropagationManager_h_
