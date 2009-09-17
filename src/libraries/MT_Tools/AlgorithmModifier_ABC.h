// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AlgorithmModifier_ABC_h_
#define __AlgorithmModifier_ABC_h_

namespace tools
{
// =============================================================================
/** @class  AlgorithmModifier_ABC
    @brief  AlgorithmModifier_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
template< typename Algorithm >
class AlgorithmModifier_ABC
{
public:
    virtual void Execute( Algorithm& algo ) const = 0;
};

} // namespace tools

#endif // __AlgorithmModifier_ABC_h_
