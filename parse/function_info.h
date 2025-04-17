#include <string>
#include <vector>

#pragma once

namespace clang {
namespace tooling {

struct LocationInfo {
  std::string file;
  unsigned line;
  unsigned column;
};

struct ParameterInfo {
  std::string name;
  std::string type;
};

struct MethodInfo {
  std::string name;
  std::string return_type;
  std::vector<ParameterInfo> parameters;
  bool is_const;
  bool is_static;
  bool is_virtual;
  std::string access;
  LocationInfo location;
  std::string comment;
};

struct FieldInfo {
  std::string name;
  std::string type;
  std::string access;  // public/protected/private
};

struct ClassInfo {
  std::string name;
  std::string kind;
  std::vector<MethodInfo> methods;
  std::vector<FieldInfo> fields;
  std::vector<std::string> bases;
  LocationInfo location;
  std::string comment;
  std::string ns;
};

struct FunctionInfo {
  std::string name;
  std::string return_type;
  std::vector<ParameterInfo> parameters;
  bool is_inline;
  std::string comment;
  LocationInfo location;
  std::string ns;
};

}  // namespace tooling
}  // namespace clang
