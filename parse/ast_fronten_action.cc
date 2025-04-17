#include "parse/ast_fronten_action.h"

#include "parse/ast_consumer.h"

namespace clang {
namespace tooling {

std::unique_ptr<ASTConsumer> ASTFrontendActionImpl::CreateASTConsumer(CompilerInstance& CI,
                                                                      StringRef) {
  return std::make_unique<ASTConsumerImpl>(&CI.getASTContext());
}

}  // namespace tooling
}  // namespace clang
