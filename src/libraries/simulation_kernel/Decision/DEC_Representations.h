// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Representations_h_
#define __DEC_Representations_h_

#include "MT_Tools/MT_Role_ABC.h"

class MIL_FragOrder;
class DEC_PathPoint;

// =============================================================================
/** @class  DEC_Representations
    @brief  DEC_Representations
*/
// Created: LDC 2009-04-03
// =============================================================================
class DEC_Representations : public MT_Role_ABC
{
public:
    typedef DEC_Representations RoleInterface;

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Representations( MT_RoleContainer& roleContainer );
    virtual ~DEC_Representations();
    //@}

    //! @name Operations
    //@{
    const std::vector< MIL_FragOrder* >& GetOrdersCategory();
    void AddToOrdersCategory( MIL_FragOrder* pObject );
    void RemoveFromOrdersCategory( MIL_FragOrder* pObject );
    void DeleteOrderRepresentation( MIL_FragOrder* pObject );

    const std::vector< DEC_PathPoint* >& GetPointsCategory();
    void AddToPointsCategory( DEC_PathPoint* pObject );
    void RemoveFromPointsCategory( DEC_PathPoint* pObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Representations( const DEC_Representations& );            //!< Copy constructor
    DEC_Representations& operator=( const DEC_Representations& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< MIL_FragOrder* >          T_OrdersRepresentationVector;
    typedef T_OrdersRepresentationVector::iterator IT_OrdersRepresentationVector;
    typedef std::vector< DEC_PathPoint* >          T_PointsRepresentationVector;
    typedef T_PointsRepresentationVector::iterator IT_PointsRepresentationVector;
    //@}

private:
    //! @name Member data
    //@{
    T_OrdersRepresentationVector orderRepresentations_;
    T_PointsRepresentationVector pointRepresentations_;
    //@}
};

#endif // __DEC_Representations_h_
