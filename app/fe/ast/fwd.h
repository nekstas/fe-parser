#pragma once

namespace ast {

class Node;
class Expression;
class Statement;
class Module;

class AtomExpresison;
class NumberExpression;
class VariableExpression;
class CallExpression;
class BinaryExpresison;

class DefineVariableStatement;
class DefineFunctionStatement;
class DefineModuleStatement;

class ImportStatement;
class CommonImportStatement;
class ImportAsStatement;
class ImportListStatement;

class Visitor;

}  // namespace ast
