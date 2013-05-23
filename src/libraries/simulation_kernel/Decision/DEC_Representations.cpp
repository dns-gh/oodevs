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
#include "Decision/DEC_PathPoint.h"

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
const std::vector< boost::shared_ptr< MIL_FragOrder > >& DEC_Representations::GetOrdersCategory()
{
    return orderRepresentations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::GetPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
const std::vector< boost::shared_ptr< DEC_PathPoint > >& DEC_Representations::GetPointsCategory()
{
    return pointRepresentations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::AddToOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::AddToOrdersCategory( boost::shared_ptr< MIL_FragOrder > pObject )
{
    orderRepresentations_.push_back( pObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::AddToPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::AddToPointsCategory( boost::shared_ptr< DEC_PathPoint > pObject )
{
    pointRepresentations_.push_back( pObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::RemoveFromOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::RemoveFromOrdersCategory( boost::shared_ptr< MIL_FragOrder > pObject )
{
    if( !orderRepresentations_.empty() )
        orderRepresentations_.erase( std::remove( orderRepresentations_.begin(), orderRepresentations_.end(), pObject ), orderRepresentations_.end() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::RemoveFromPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::RemoveFromPointsCategory( boost::shared_ptr< DEC_PathPoint > pObject )
{
    if( !pointRepresentations_.empty() )
        pointRepresentations_.erase( std::remove( pointRepresentations_.begin(), pointRepresentations_.end(), pObject ), pointRepresentations_.end() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::DeleteOrderRepresentation
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::DeleteOrderRepresentation( boost::shared_ptr< MIL_FragOrder > pObject )
{
    if( !orderRepresentations_.empty() )
        orderRepresentations_.erase( std::remove( orderRepresentations_.begin(), orderRepresentations_.end(), pObject ), orderRepresentations_.end() );
}
