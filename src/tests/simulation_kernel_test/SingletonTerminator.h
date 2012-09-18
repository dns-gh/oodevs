// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_KERNEL_TEST_SINGLETONTERMINATOR_H
#define SIMULATION_KERNEL_TEST_SINGLETONTERMINATOR_H

class StubMIL_Time_ABC;

class SingletonTerminator
{
public:
     SingletonTerminator( StubMIL_Time_ABC* time );
    ~SingletonTerminator();

private:
    StubMIL_Time_ABC* time_;
};

#endif  //SIMULATION_KERNEL_TEST_SINGLETONTERMINATOR_H