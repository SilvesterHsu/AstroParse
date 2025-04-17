#include "parse/ast_consumer.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "parse/util.h"

namespace clang {
namespace tooling {
namespace {
using nlohmann::json;
}  // namespace

ASTConsumerImpl::ASTConsumerImpl(const ASTContext* context) : visitor_(context) {
}

void ASTConsumerImpl::HandleTranslationUnit(ASTContext& Context) {
  visitor_.TraverseDecl(Context.getTranslationUnitDecl());
  json j;
  j["classes"] = visitor_.classes();
  j["functions"] = visitor_.functions();
  std::ofstream o("ast_output.json");
  if (o.is_open()) {
    o << std::setw(4) << j << std::endl;
    o.close();
    llvm::outs() << "AST output written to ast_output.json\n";
  } else {
    llvm::errs() << "Error: Unable to open output file for writing.\n";
  }
}

}  // namespace tooling
}  // namespace clang
