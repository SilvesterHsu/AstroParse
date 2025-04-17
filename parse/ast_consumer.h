#pragma once

#include <clang/AST/ASTConsumer.h>

#include "parse/ast_visitor.h"

namespace clang {
namespace tooling {

class ASTConsumerImpl : public ASTConsumer {
 public:
  explicit ASTConsumerImpl(const ASTContext* context);

 public:
  void HandleTranslationUnit(ASTContext& Context) override;

 private:
  ASTVisitor visitor_;
};

}  // namespace tooling
}  // namespace clang
