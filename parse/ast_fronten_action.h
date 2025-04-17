#pragma once

#include <clang/Frontend/FrontendActions.h>

#include "parse/ast_visitor.h"

namespace clang {
namespace tooling {

class ASTFrontendActionImpl : public ASTFrontendAction {
 public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef InFile) override;
};

}  // namespace tooling
}  // namespace clang
