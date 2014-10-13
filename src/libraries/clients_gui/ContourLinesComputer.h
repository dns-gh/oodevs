// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ContourLinesComputer_h
#define ContourLinesComputer_h

#include <tools/thread/ThreadPool.h>
#include <boost/optional/optional.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class DetectionMap;
}

namespace gui
{
    class ContourLinesObserver;

// =============================================================================
/** @class  ContourLinesComputer
    @brief  ContourLinesComputer
*/
// Created: ABR 2014-07-01
// =============================================================================
class ContourLinesComputer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ContourLinesComputer( kernel::Controller& controller,
                                   const kernel::DetectionMap& map );
    virtual ~ContourLinesComputer();
    //@}

    //! @name Operations
    //@{
    void SetHeight( int height );
    int GetHeight() const;
    unsigned int CreateCallList();

    void SetContourLinesObserver( const std::shared_ptr< ContourLinesObserver >& observer );
    const std::shared_ptr< ContourLinesObserver >& GetContourLinesObserver() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Reset();
    void Conrec();
    void UpdateObservers( short );
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< T_PointVector> > T_Contours[ 4 ];
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::shared_ptr< ContourLinesObserver > observer_;
    const kernel::DetectionMap& map_;
    std::unique_ptr< tools::thread::ThreadPool > thread_;
    boost::mutex mutex_;
    bool stopThread_;
    bool threadRunning_;
    bool computed_;
    bool valid_;
    T_Contours loops_;
    int height_;
    unsigned int callList_;
    //@}
};

} //! namespace gui

#endif // ContourLinesComputer_h
