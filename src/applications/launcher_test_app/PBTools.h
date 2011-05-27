// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef __LAUNCHER_TEST_APP_PBTOOLS_H__
#define __LAUNCHER_TEST_APP_PBTOOLS_H__

#include <iosfwd>
#include <map>

namespace xml
{
    class xistream;
}

namespace google
{
namespace protobuf
{
    class Message;
    class FieldDescriptor;
    class Reflection;
}
}
namespace launcher_test_app
{
// =============================================================================
/** @class  PBTools
    @brief  PBTools
*/
// Created: AHC 2011-05-27
// =============================================================================
class PBTools
{
public:
    static void ToXML(std::ostream& os, const google::protobuf::Message& msg);
    static google::protobuf::Message* FromXML(xml::xistream& xis);
    static void FromXML(xml::xistream& xis, google::protobuf::Message& msg);

private:
    static void ToXML(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& desc,  const google::protobuf::Reflection& refl);
    static void FieldFromXML(xml::xistream& xis, google::protobuf::Message& msg, std::map<std::string, int>& indexMap);
};

}

#endif // __LAUNCHER_TEST_APP_PBTOOLS_H__
