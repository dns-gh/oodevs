// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Representations.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Decision/Path/DEC_PathPoint.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Representations )

// -----------------------------------------------------------------------------
// Name: DEC_Representations constructor
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
DEC_Representations::DEC_Representations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations destructor
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
DEC_Representations::~DEC_Representations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::GetOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
const std::vector< MIL_FragOrder* >& DEC_Representations::GetOrdersCategory()
{
    return orderRepresentations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::GetPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
const std::vector< DEC_PathPoint* >& DEC_Representations::GetPointsCategory()
{
    return pointRepresentations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::AddToOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::AddToOrdersCategory( MIL_FragOrder* pObject )
{
    orderRepresentations_.push_back( pObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::AddToPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::AddToPointsCategory( DEC_PathPoint* pObject )
{
    pointRepresentations_.push_back( pObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::RemoveFromOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::RemoveFromOrdersCategory( MIL_FragOrder* pObject )
{
    for( IT_OrdersRepresentationVector it = orderRepresentations_.begin(); it != orderRepresentations_.end(); ++it )
    {
        if( *it == pObject )
        {
            orderRepresentations_.erase( it );
            break;
        }
    }
}
// -----------------------------------------------------------------------------
// Name: DEC_Representations::RemoveFromPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::RemoveFromPointsCategory( DEC_PathPoint* pObject )
{
    for( IT_PointsRepresentationVector it = pointRepresentations_.begin(); it != pointRepresentations_.end(); ++it )
    {
        if( *it == pObject )
        {
            pointRepresentations_.erase( it );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::DeleteOrderRepresentation
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::DeleteOrderRepresentation( MIL_FragOrder* pObject )
{    
    for( IT_OrdersRepresentationVector oit = orderRepresentations_.begin(); oit != orderRepresentations_.end(); ++oit )
    {
        if( *oit == pObject )
        {
            orderRepresentations_.erase( oit );
            delete pObject;
            return;
        }
    }
}
