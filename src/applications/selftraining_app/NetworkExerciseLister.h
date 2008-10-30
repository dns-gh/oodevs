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

#include <boost/asio.hpp>
#include "ExerciseLister_ABC.h"

namespace boost
{
    class thread;
}

class ExerciseList;
class Config;

// =============================================================================
/** @class  NetworkExerciseLister
    @brief  NetworkExerciseLister
*/
// Created: LDC 2008-10-24
// =============================================================================
class NetworkExerciseLister : public ExerciseLister_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NetworkExerciseLister( const Config& config, const std::string& subDir ="" );
    virtual ~NetworkExerciseLister();
    //@}

    //! @name Operations
    //@{
    void DownloadExercises( const std::string& host, unsigned int port );
    void AddList( ExerciseList* list );
    virtual void ListExercises( QStringList& list ) const;
    virtual unsigned short GetPort( const QString& exercise ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NetworkExerciseLister( const NetworkExerciseLister& );            //!< Copy constructor
    NetworkExerciseLister& operator=( const NetworkExerciseLister& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnSendExercisesRequest( const boost::system::error_code& error );
    void Send( const std::string& host, unsigned int port );
    void OnReceive( const boost::system::error_code& error, size_t bytes_received );
    void RunNetwork();
    void ClearListeners();
    void UpdateListeners( const std::string& scenario, const std::string& port );
    //@}

    //! @name Types
    //@{
    typedef std::vector<ExerciseList *> T_Lists;
    typedef T_Lists::const_iterator     CIT_Lists;
    typedef std::map< std::string, unsigned short > T_PortDictionary;
    typedef T_PortDictionary::const_iterator     CIT_PortDictionary;
    //@}

private:
    //! @name Member data
    //@{
    const Config&                  config_;
    std::string                    subDir_;
    boost::asio::io_service        network_;
    boost::asio::ip::udp::socket   socket_;
    char                           answer_[1024];
    std::auto_ptr< boost::thread > thread_;
    T_Lists                        lists_;
    T_PortDictionary               exercises_;
    //@}
};

#endif // __ExerciseLister_h_
