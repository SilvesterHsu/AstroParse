#pragma once

#include <clang/AST/RecursiveASTVisitor.h>

#include "parse/function_info.h"

namespace clang {
namespace tooling {

class ASTVisitor : public RecursiveASTVisitor<ASTVisitor> {
 public:
  explicit ASTVisitor(const ASTContext* context);
  ~ASTVisitor() = default;

 public:
  bool VisitCXXRecordDecl(const CXXRecordDecl* decl);
  bool VisitFunctionDecl(const FunctionDecl* func_decl);

  const std::vector<ClassInfo>& classes() const;
  const std::vector<FunctionInfo>& functions() const;

 private:
  const ASTContext* context_ = nullptr;
  std::vector<ClassInfo> classes_;
  std::vector<FunctionInfo> functions_;
};

}  // namespace tooling
}  // namespace clang
