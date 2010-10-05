// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseLister_h_
#define __ExerciseLister_h_

#include "ExerciseLister_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <map>

namespace frontend
{
    class Exercises;
    class LauncherClient;
}

namespace kernel
{
    class Controllers;
}

class Config;
class ExerciseList;
class QTimer;

// =============================================================================
/** @class  NetworkExerciseLister
    @brief  NetworkExerciseLister
*/
// Created: LDC 2008-10-24
// =============================================================================
class NetworkExerciseLister : public QObject
                            , public ExerciseLister_ABC
                            , public tools::Observer_ABC
                            , public tools::ElementObserver_ABC< frontend::Exercises >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             NetworkExerciseLister( QObject* parent, kernel::Controllers& controllers, const Config& config, const std::string& subDir ="" );
    virtual ~NetworkExerciseLister();
    //@}

    //! @name Operations
    //@{
    void QueryExercises( const std::string& host, unsigned int port, ExerciseList& list );
    virtual void ListExercises( QStringList& list ) const;
    virtual unsigned short GetPort( const QString& exercise ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NetworkExerciseLister( const NetworkExerciseLister& );            //!< Copy constructor
    NetworkExerciseLister& operator=( const NetworkExerciseLister& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const frontend::Exercises& exercises );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_PortDictionary;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Config& config_;
    std::string subDir_;
    std::auto_ptr< frontend::LauncherClient > launcher_;
    ExerciseList* list_;
    T_PortDictionary exercises_;
    QTimer* timer_;
    bool pendingQuery_;
    //@}
};

#endif // __ExerciseLister_h_
