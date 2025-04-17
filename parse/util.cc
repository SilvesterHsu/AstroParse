#include "parse/util.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/RawCommentList.h"

namespace clang {
namespace tooling {
namespace {
using nlohmann::json;
}  // namespace

bool IsProjectFile(const SourceManager& src_manager, const SourceLocation& src_loc) {
  std::string file_path = src_manager.getFilename(src_loc).str();
  return !file_path.empty() && !(file_path.rfind("/usr", 0) == 0) &&
         !(file_path.rfind("/opt", 0) == 0) && !(file_path.rfind("//usr/include", 0) == 0) &&
         !(file_path.rfind("external", 0) == 0);
}

std::string GetEnclosingNamespace(const Decl* decl) {
  std::string ns;
  const DeclContext* decl_context = decl->getDeclContext();
  while (decl_context && !isa<TranslationUnitDecl>(decl_context)) {
    if (const NamespaceDecl* ns_decl = dyn_cast<NamespaceDecl>(decl_context)) {
      std::string cur =
          ns_decl->isAnonymousNamespace() ? "(anonymous)" : ns_decl->getNameAsString();
      if (!ns.empty()) {
        ns = cur + "::" + ns;
      } else {
        ns = cur;
      }
    }
    decl_context = decl_context->getParent();
  }
  return ns;
}

std::string GetComment(const Decl* decl, const ASTContext& context) {
  const RawComment* comment = context.getRawCommentForAnyRedecl(decl);
  if (comment) {
    return comment->getRawText(context.getSourceManager()).str();
  }
  return "";
}

std::string AccessSpecifierToString(const AccessSpecifier& access) {
  switch (access) {
    case AS_public:
      return "public";
    case AS_protected:
      return "protected";
    case AS_private:
      return "private";
    default:
      return "none";
  }
}

void to_json(json& j, const LocationInfo& loc) {
  j = json{{"file", loc.file}, {"line", loc.line}, {"column", loc.column}};
}

void to_json(json& j, const ParameterInfo& param) {
  j = json{{"name", param.name}, {"type", param.type}};
}

void to_json(json& j, const MethodInfo& method) {
  j = json{{"name", method.name},
           {"return_type", method.return_type},
           {"parameters", method.parameters},
           {"is_const", method.is_const},
           {"is_static", method.is_static},
           {"is_virtual", method.is_virtual},
           {"access", method.access},
           {"location", method.location},
           {"comment", method.comment}};
}

void to_json(json& j, const FieldInfo& field) {
  j = json{{"name", field.name}, {"type", field.type}, {"access", field.access}};
}

void to_json(json& j, const ClassInfo& cls) {
  j = json{{"name", cls.name},       {"kind", cls.kind},   {"methods", cls.methods},
           {"fields", cls.fields},   {"bases", cls.bases}, {"location", cls.location},
           {"comment", cls.comment}, {"namespace", cls.ns}};
}

void to_json(json& j, const FunctionInfo& func) {
  j = json{{"name", func.name},
           {"return_type", func.return_type},
           {"parameters", func.parameters},
           {"is_inline", func.is_inline},
           {"comment", func.comment},
           {"location", func.location},
           {"namespace", func.ns}};
}

}  // namespace tooling
}  // namespace clang
