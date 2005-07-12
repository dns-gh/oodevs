// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_DynaObject_ABC.h $
// $Author: Jvt $
// $Modtime: 5/04/05 18:36 $
// $Revision: 3 $
// $Workfile: TER_DynaObject_ABC.h $
//
// *****************************************************************************

#ifndef __TER_DynaObject_ABC_h_
#define __TER_DynaObject_ABC_h_

#include "TER_Localisation.h"
#include "TER_ObjectPositionHint.h"

// =============================================================================
/** @class  TER_DynaObject_ABC
    @brief  TER_DynaObject_ABC
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_DynaObject_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_DynaObject_ABC* >   T_DynaObjectVector;
    typedef T_DynaObjectVector::iterator        IT_DynaObjectVector;
    typedef T_DynaObjectVector::const_iterator CIT_DynaObjectVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_DynaObject_ABC();
    virtual ~TER_DynaObject_ABC();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( boost::archive::binary_iarchive&, const uint );
    void save( boost::archive::binary_oarchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Initialize( const TER_Localisation& localisation );
    void Terminate();

    bool IsInside             ( const MT_Vector2D& vPos ) const;
    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions ) const;
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    //@}

    //! @name Modifiers
    //@{
    void UpdateLocalisation( const TER_Localisation& newLocalisation );
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_DynaObject_ABC( const TER_DynaObject_ABC& );            //!< Copy constructor
    TER_DynaObject_ABC& operator=( const TER_DynaObject_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void InsertInWorld();
    void RemoveFromWorld();
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation location_;
    TER_ObjectPositionHint hint_;
    //@}
};

#endif // __TER_DynaObject_ABC_h_
