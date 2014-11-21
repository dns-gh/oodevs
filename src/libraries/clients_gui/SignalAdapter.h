// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SignalAdapter_h_
#define __SignalAdapter_h_

namespace gui
{
// =============================================================================
/** @class  SignalAdapter
    @brief  A Qt signal to nullary functor adapter
*/
// Created: MCO 2013-07-05
// =============================================================================
class SignalAdapter : public QObject
{
    Q_OBJECT

public:
    SignalAdapter( QObject* sender, const char* signal, const std::function< void() >& f );

public slots:
    void Handle();
    void Destroy();

private:
      std::function< void() > f_;
};

bool connect( QObject* sender, const char* signal, const std::function< void() >& f );

}

#endif // __SignalAdapter_h_
