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
    @brief  A Qt signal to nulary functor adapter
*/
// Created: MCO 2013-07-05
// =============================================================================
class SignalAdapter : public QObject
{
    Q_OBJECT

public:
    SignalAdapter( QObject* parent, const std::function< void() >& f );

public slots:
    void Handle();

private:
      std::function< void() > f_;
};

inline bool connect( QObject* sender, const char * signal, const std::function< void() >& f )
{
    return QObject::connect( sender, signal, new gui::SignalAdapter( sender, f ), SLOT( Handle() ) );
}
}

#endif // __SignalAdapter_h_
