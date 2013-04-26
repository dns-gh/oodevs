// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CONTEXT_H__
#define CONTEXT_H__

#include <timeline_core/api.h>
#pragma warning( push, 0 )
#include <cef_base.h>
#pragma warning( pop )

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
namespace core
{
class Context : public Context_ABC

{
public:
             Context( const Configuration& cfg );
    virtual ~Context();

    struct Handler;

private:
    const Configuration cfg_;
    std::auto_ptr< tools::ipc::Device > device_;
    CefRefPtr< Handler > handler_;
};
}
}

#endif//CONTEXT_H__
