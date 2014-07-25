// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef DEC_PathFind_Manager_ABC_h
#define DEC_PathFind_Manager_ABC_h

#include <boost/noncopyable.hpp>

namespace sword
{
    class Pathfind;
}

class DEC_PathComputer_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_PathFind_Manager_ABC
    @brief  DEC_PathFind_Manager_ABC
*/
// Created: SLI 2014-07-24
// =============================================================================
class DEC_PathFind_Manager_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathFind_Manager_ABC() {}
    virtual ~DEC_PathFind_Manager_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double Update() = 0;
    virtual void UpdateInSimulationThread() = 0;
    virtual void StartCompute( const boost::shared_ptr< DEC_PathComputer_ABC >& pPath, const sword::Pathfind& pathfind ) = 0;
    virtual void CancelJobForUnit( MIL_Agent_ABC* pion ) = 0;
    virtual void CleanPathAfterComputation( double duration ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual int GetCurrentThread() const = 0;
    virtual unsigned int GetNbrShortRequests() const = 0;
    virtual unsigned int GetNbrLongRequests() const = 0;
    virtual unsigned int GetNbrTreatedRequests() const = 0;
    virtual unsigned int GetMaxComputationDuration() const = 0;
    //@}
};

#endif // DEC_PathFind_Manager_ABC_h
