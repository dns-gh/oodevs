// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Tools/Position.h"

namespace TEST {

    class Team;

// =============================================================================
/** @class  Object
    @brief  Object
    @par    Using example
    @code
    Object;
    @endcode
*/
// Created: SBO 2005-08-08
// =============================================================================
class Object
{

public:
    //! @name Constructors/Destructor
    //@{
             Object( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    static void    Initialize();
    static void    Terminate ();
    static Object* Find      ( T_EntityId nId );
    static void    Register  ( Object& automat );
    static void    Unregister( Object& automat );
    //@}

    //! @name Accessors
    //@{
          T_EntityId GetId  () const;
	const Team&      GetTeam() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< T_EntityId, Object* > T_ObjectMap;
	typedef T_ObjectMap::const_iterator	    CIT_ObjectMap;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId             nId_;
    Team*                  pTeam_;
    uint                   nType_;
    uint                   nTypeLocalisation_;
    T_PositionVector positionVector_;

    // global automat list
    static T_ObjectMap  objects_;
    //@}
};

}

#include "Object.inl"

#endif // __Object_h_
