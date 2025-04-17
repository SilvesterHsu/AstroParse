#include "parse/ast_visitor.h"

#include "parse/util.h"

namespace clang {
namespace tooling {
ASTVisitor::ASTVisitor(const ASTContext* context) : context_(context) {
}

bool ASTVisitor::VisitCXXRecordDecl(const CXXRecordDecl* decl) {
  if (decl->isThisDeclarationADefinition() &&
      IsProjectFile(context_->getSourceManager(), decl->getLocation())) {
    ClassInfo cls;
    cls.name = decl->getNameAsString();
    cls.kind = decl->getKindName();
    cls.comment = GetComment(decl, *context_);
    cls.ns = GetEnclosingNamespace(decl);

    SourceLocation loc = decl->getLocation();
    const SourceManager& src_manager = context_->getSourceManager();
    PresumedLoc presu_loc = src_manager.getPresumedLoc(loc);
    if (!presu_loc.isInvalid()) {
      cls.location.file = presu_loc.getFilename();
      cls.location.line = presu_loc.getLine();
      cls.location.column = presu_loc.getColumn();
    }

    for (const auto& base : decl->bases()) {
      std::string baseType = base.getType().getAsString();
      cls.bases.emplace_back(baseType);
    }

    for (auto field : decl->fields()) {
      FieldInfo f;
      f.name = field->getNameAsString();
      f.type = field->getType().getAsString();
      f.access = AccessSpecifierToString(field->getAccess());
      cls.fields.emplace_back(f);
    }

    for (auto method : decl->methods()) {
      if (!method->isImplicit()) {
        MethodInfo m;
        m.name = method->getNameAsString();
        m.return_type = method->getReturnType().getAsString();
        m.is_const = method->isConst();
        m.is_static = method->isStatic();
        m.is_virtual = method->isVirtualAsWritten();
        m.access = AccessSpecifierToString(method->getAccess());
        m.comment = GetComment(method, *context_);

        SourceLocation src_loc = method->getLocation();
        PresumedLoc presu_loc = src_manager.getPresumedLoc(src_loc);
        if (!presu_loc.isInvalid()) {
          m.location.file = presu_loc.getFilename();
          m.location.line = presu_loc.getLine();
          m.location.column = presu_loc.getColumn();
        }

        for (auto param : method->parameters()) {
          ParameterInfo p;
          p.name = param->getNameAsString();
          p.type = param->getOriginalType().getAsString();
          m.parameters.emplace_back(p);
        }
        cls.methods.emplace_back(m);
      }
    }
    classes_.emplace_back(cls);
  }
  return true;
}

bool ASTVisitor::VisitFunctionDecl(const FunctionDecl* func_decl) {
  if (isa<CXXMethodDecl>(func_decl)) {
    return true;
  }
  if (!func_decl->isThisDeclarationADefinition()) {
    return true;
  }
  const SourceManager& src_manager = context_->getSourceManager();
  if (!IsProjectFile(src_manager, func_decl->getLocation())) {
    return true;
  }

  FunctionInfo func;
  func.name = func_decl->getNameAsString();
  func.return_type = func_decl->getReturnType().getAsString();
  func.is_inline = func_decl->isInlineSpecified();
  func.comment = GetComment(func_decl, *context_);
  func.ns = GetEnclosingNamespace(func_decl);

  SourceLocation loc = func_decl->getLocation();
  PresumedLoc presu_loc = src_manager.getPresumedLoc(loc);
  if (!presu_loc.isInvalid()) {
    func.location.file = presu_loc.getFilename();
    func.location.line = presu_loc.getLine();
    func.location.column = presu_loc.getColumn();
  }

  for (auto param : func_decl->parameters()) {
    ParameterInfo p;
    p.name = param->getNameAsString();
    p.type = param->getOriginalType().getAsString();
    func.parameters.emplace_back(p);
  }

  functions_.emplace_back(func);
  return true;
}

const std::vector<ClassInfo>& ASTVisitor::classes() const {
  return classes_;
}

const std::vector<FunctionInfo>& ASTVisitor::functions() const {
  return functions_;
}

}  // namespace tooling
}  // namespace clang
