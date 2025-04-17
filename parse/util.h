#pragma once

#include "clang/AST/DeclBase.h"
#include "clang/Basic/SourceManager.h"
#include "nlohmann/json.hpp"

#include "parse/function_info.h"

namespace clang {
namespace tooling {

bool IsProjectFile(const SourceManager& src_manager, const SourceLocation& src_loc);

std::string GetEnclosingNamespace(const Decl* decl);

std::string GetComment(const Decl* decl, const ASTContext& context);

std::string AccessSpecifierToString(const AccessSpecifier& access);

void to_json(nlohmann::json& j, const LocationInfo& loc);

void to_json(nlohmann::json& j, const ParameterInfo& param);

void to_json(nlohmann::json& j, const MethodInfo& method);

void to_json(nlohmann::json& j, const FieldInfo& field);

void to_json(nlohmann::json& j, const ClassInfo& cls);

void to_json(nlohmann::json& j, const FunctionInfo& func);

}  // namespace tooling
}  // namespace clang
