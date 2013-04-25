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

#include <timeline_ui/api.h>

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{

class Context : public Context_ABC
{
public:
             Context( const Configuration& cfg );
    virtual ~Context();

private:
    std::auto_ptr< tools::ipc::Device > device_;
};

}

#endif//CONTEXT_H__
