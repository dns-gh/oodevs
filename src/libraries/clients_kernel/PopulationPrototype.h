// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PopulationPrototype_h_
#define __PopulationPrototype_h_

namespace kernel
{
    class PopulationType;

// =============================================================================
/** @class  PopulationPrototype
    @brief  PopulationPrototype
*/
// Created: LDC 2010-10-25
// =============================================================================
struct PopulationPrototype
{
public:
    PopulationPrototype() : type_( 0 ), number_( 0 ) {}
    ~PopulationPrototype() {}
    const PopulationType* type_;
    int number_;
};
}

#endif // __PopulationPrototype_h_
