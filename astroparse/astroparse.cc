#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include "gflags/gflags.h"

#include "parse/ast_fronten_action.h"

static llvm::cl::OptionCategory ToolCategory("ast-parser options");

namespace {
using clang::tooling::ASTFrontendActionImpl;
using clang::tooling::ClangTool;
using clang::tooling::CommonOptionsParser;
using clang::tooling::newFrontendActionFactory;

// DEFINE_string(file, "", "File to parse");
// DEFINE_string(output, "", "Output file");
}  // namespace

int32_t main(int32_t argc, const char** argv) {
  auto expected_parser = CommonOptionsParser::create(argc, argv, ToolCategory);
  if (!expected_parser) {
    llvm::errs() << "Error: " << llvm::toString(expected_parser.takeError()) << "\n";
    return 1;
  }
  CommonOptionsParser& optionsParser = expected_parser.get();
  ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());
  int32_t result = tool.run(newFrontendActionFactory<ASTFrontendActionImpl>().get());

  return result;
}
