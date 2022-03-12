#ifndef CA_EXPRESSIONS_H
#define CA_EXPRESSIONS_H

#include <istream>
#include <string>
#include <vector>

namespace ca {

#define OP_TYPES \
	X(literal) \
	X(plus) \
	X(minus) \
	X(ternary) \
	X(min) \
	X(max) \

enum struct OpType : short {
#define X(name) name,
	OP_TYPES
#undef X
	SIZE
};

namespace ops::literal {
	struct OpData {
		int number;
	};
}
namespace ops::plus {
	struct OpData {
	};
}
namespace ops::minus {
	struct OpData {
	};
}
namespace ops::ternary {
	struct OpData {
	};
}
namespace ops::min {
	struct OpData {
		unsigned argc;
	};
}
namespace ops::max {
	struct OpData {
		unsigned argc;
	};
}
union OpData {
#define X(name) ops::name::OpData name;
		OP_TYPES
#undef X
};

struct Op {
	OpType type;
	OpData data;
};

using Expr = std::vector<Op>;

struct CaExpressionsDriver {
	using Expr = ca::Expr;

	static int Compute(Expr& e);
	static std::string AsString(Expr& e);
	static void Save(Expr& e, std::ostream& s);
	static Expr Load(std::istream &s);

	static Expr ConstantExpression(int value);
	static Expr BinaryOperatorExpression(char op, Expr l, Expr r);
	static Expr TernaryOperatorExpression(Expr c, Expr t, Expr f);
	static Expr FunctionCallExpression(std::string f, const std::vector<Expr>& args);
};

}
#endif
