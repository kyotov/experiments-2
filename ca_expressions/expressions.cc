#include "expressions.h"

// Building and testing:
// cmake -B build -DCMAKE_BUILD_TYPE=Release
// clear && cmake --build ./build --config Release --target all && build/test_expressions/benchmark_expressions

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>

using namespace ca;

#include "utils.h"

static const bool debug_print = false;
static const bool fastpath_exceptions = false;

enum struct Implementation : short {
	Recursive,
	EvaluateOnStack,
	EvaluateOnHeap,
};

//static const Implementation implementation = Implementation::EvaluateOnStack;
static const Implementation implementation = Implementation::EvaluateOnHeap;

template <typename T>
using Uninited = char;

template <typename T>
inline Uninited<T>* cast(T* t) {
	return reinterpret_cast<Uninited<T>*>(t);
}

template <typename T, typename... Args>
static constexpr T* emplace(Uninited<T>* result, Args&&... args) {
	return std::construct_at(reinterpret_cast<T*>(result), std::forward<Args>(args)...);
}

namespace ca::ops {
	static inline int compute(const Op** op);
	static inline std::string string_infix(const Op** op);
	static inline std::string string_save(const Op** op);
}

namespace ca::ops::literal {
	static inline std::string prefix() {
		return "C";
	}
	static inline size_t init_args() {
		return 1;
	}
	static inline Op init(int* args) {
		return {OpType::literal, {.literal = {args[0]}}};
	}
	static inline size_t eval_args(const Op* op) {
		(void)op;
		return 0;
	}
	static inline int compute(const Op** op) {
		return (*op)->data.literal.number;
	}
	static inline void compute(const Op* op, const int* args, Uninited<int>* res) {
		(void)args;
		emplace<int>(res, op->data.literal.number);
	}
	static std::string string_infix(const Op** op) {
		return "(" + std::to_string((*op)->data.literal.number) + ")";
	}
	static void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
		(void)args;
		std::string num = std::to_string(op->data.literal.number);
		emplace<std::string>(res, "(" + num + ")");
	}
	static std::string string_save(const Op** op) {
		return "C " + std::to_string((*op)->data.literal.number);
	}
}

namespace ca::ops::plus {
	static inline std::string prefix() {
		return "BOp +";
	}
	static inline size_t init_args() {
		return 0;
	}
	static inline Op init(int* args) {
		(void)args;
		return {OpType::plus, {}};
	}
	static inline size_t eval_args(const Op* op) {
		(void)op;
		return 2;
	}
	static inline int compute(const Op** op) {
		++(*op);
		int a = ca::ops::compute(op);
		++(*op);
		int b = ca::ops::compute(op);
		return a + b;
	}
	static inline void compute(const Op* op, const int* args, Uninited<int>* res) {
		(void)op;
		emplace<int>(res, args[0] + args[1]);
	}
	static std::string string_infix(const Op** op) {
		++(*op);
		std::string a = ca::ops::string_infix(op);
		++(*op);
		std::string b = ca::ops::string_infix(op);
		return "(" + a + "+" + b + ")";
	}
	static void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
		(void)op;
		emplace<std::string>(res, "(" + args[0] + "+" + args[1] + ")");
	}
	static std::string string_save(const Op** op) {
		++(*op);
		std::string a = ca::ops::string_save(op);
		++(*op);
		std::string b = ca::ops::string_save(op);
		return "BOp + " + a + " " + b + " ";
	}
}

namespace ca::ops::minus {
	static inline std::string prefix() {
		return "BOp -";
	}
	static inline size_t init_args() {
		return 0;
	}
	static inline Op init(int* args) {
		(void)args;
		return {OpType::minus, {}};
	}
	static inline size_t eval_args(const Op* op) {
		(void)op;
		return 2;
	}
	static inline int compute(const Op** op) {
		++(*op);
		int a = ca::ops::compute(op);
		++(*op);
		int b = ca::ops::compute(op);
		return a - b;
	}
	static inline void compute(const Op* op, const int* args, Uninited<int>* res) {
		(void)op;
		emplace<int>(res, args[0] - args[1]);
	}
	static std::string string_infix(const Op** op) {
		++(*op);
		std::string a = ca::ops::string_infix(op);
		++(*op);
		std::string b = ca::ops::string_infix(op);
		return "(" + a + "-" + b + ")";
	}
	static void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
		(void)op;
		emplace<std::string>(res, "(" + args[0] + "-" + args[1] + ")");
	}
	static std::string string_save(const Op** op) {
		++(*op);
		std::string a = ca::ops::string_save(op);
		++(*op);
		std::string b = ca::ops::string_save(op);
		return "BOp - " + a + " " + b + " ";
	}
}

namespace ca::ops::ternary {
	static inline std::string prefix() {
		return "TOp";
	}
	static inline size_t init_args() {
		return 0;
	}
	static inline Op init(int* args) {
		(void)args;
		return {OpType::ternary, {}};
	}
	static inline size_t eval_args(const Op* op) {
		(void)op;
		return 3;
	}
	static inline int compute(const Op** op) {
		++(*op);
		int a = ca::ops::compute(op);
		++(*op);
		int b = ca::ops::compute(op);
		++(*op);
		int c = ca::ops::compute(op);
		return a ? b : c;
	}
	static inline int compute(const Op* op, const int* args) {
		(void)op;
		return args[0] ? args[1] : args[2];
	}
	static inline void compute(const Op* op, const int* args, Uninited<int>* res) {
		(void)op;
		emplace<int>(res, args[0] ? args[1] : args[2]);
	}
	static std::string string_infix(const Op** op) {
		++(*op);
		std::string a = ca::ops::string_infix(op);
		++(*op);
		std::string b = ca::ops::string_infix(op);
		++(*op);
		std::string c = ca::ops::string_infix(op);
		return "(" + a + "?" + b + ":" + c + ")";
	}
	static void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
		(void)op;
		emplace<std::string>(res, "(" + args[0] + "?" + args[1] + ":" + args[2] + ")");
	}
	static std::string string_save(const Op** op) {
		++(*op);
		std::string a = ca::ops::string_save(op);
		++(*op);
		std::string b = ca::ops::string_save(op);
		++(*op);
		std::string c = ca::ops::string_save(op);
		return "TOp " + a + " " + b + " " + c + " ";
	}
}

namespace ca::ops::min {
	static inline std::string prefix() {
		return "Fx min";
	}
	static inline size_t init_args() {
		return 1;
	}
	static inline Op init(int* args) {
		unsigned argc;
		if (args[0] < 0) throw std::invalid_argument("negative argument count for min");
		else argc = static_cast<decltype(argc)>(args[0]);
		return {OpType::min, {.min = {argc}}};
	}
	static inline size_t eval_args(const Op* op) {
		return op->data.min.argc;
	}
	static inline int compute(const Op** op) {
		size_t argc = eval_args(*op);
		int min = std::numeric_limits<decltype(min)>::max();
		while (argc--) {
			++(*op);
			min = std::min(min, ca::ops::compute(op));
		}
		return min;
	}
	static inline void compute(const Op* op, const int* args, Uninited<int>* res) {
		size_t argc = eval_args(op);
		const int* min = std::min_element(args, args+argc);
		if (fastpath_exceptions && min == args+argc) [[unlikely]] {
			throw std::invalid_argument("no minimum");
		}
		emplace<int>(res, *min);
	}
	static std::string string_infix(const Op** op) {
		size_t argc = eval_args(*op);
		std::string res = "min(";
		while (argc--) {
			++(*op);
			res += ca::ops::string_infix(op);
			if (argc) res += ",";
		}
		return res + ")";
	}
	static void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
		size_t argc = eval_args(op);
		std::string str = "min(";
		while (argc--) {
			str += *args;
			++args;
			if (argc) str += ",";
		}
		emplace<std::string>(res, str + ")");
	}
	static std::string string_save(const Op** op) {
		size_t argc = eval_args(*op);
		std::string res = "Fx min " + std::to_string(argc) + " ";
		while (argc--) {
			++(*op);
			res += ca::ops::string_save(op);
			if (argc) res += " ";
		}
		return res + ")";
	}
}

namespace ca::ops::max {
	static inline std::string prefix() {
		return "Fx max";
	}
	static inline size_t init_args() {
		return 1;
	}
	static inline Op init(int* args) {
		unsigned argc;
		if (args[0] < 0) throw std::invalid_argument("negative argument count for max");
		else argc = static_cast<decltype(argc)>(args[0]);
		return {OpType::max, {.max = {argc}}};
	}
	static inline size_t eval_args(const Op* op) {
		return op->data.max.argc;
	}
	static inline int compute(const Op** op) {
		size_t argc = eval_args(*op);
		int max = std::numeric_limits<decltype(max)>::min();
		while (argc--) {
			++(*op);
			max = std::max(max, ca::ops::compute(op));
		}
		return max;
	}
	static inline void compute(const Op* op, const int* args, Uninited<int>* res) {
		size_t argc = eval_args(op);
		const int* max = std::max_element(args, args+argc);
		if (fastpath_exceptions && max == args+argc) [[unlikely]] {
			throw std::invalid_argument("no maximum");
		}
		emplace<int>(res, *max);
	}
	static std::string string_infix(const Op** op) {
		size_t argc = eval_args(*op);
		std::string res = "max(";
		while (argc--) {
			++(*op);
			res += ca::ops::string_infix(op);
			if (argc) res += ",";
		}
		return res + ")";
	}
	static void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
		size_t argc = eval_args(op);
		std::string str = "max(";
		while (argc--) {
			str += *args;
			++args;
			if (argc) str += ",";
		}
		emplace<std::string>(res, str + ")");
	}
	static std::string string_save(const Op** op) {
		size_t argc = eval_args(*op);
		std::string res = "Fx max " + std::to_string(argc) + " ";
		while (argc--) {
			++(*op);
			res += ca::ops::string_save(op);
			if (argc) res += " ";
		}
		return res + ")";
	}
}

namespace ca::ops {
static inline std::string prefix(OpType type) {
	switch (type) {
#define X(name) case OpType::name: return name::prefix();
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("prefix"); else return nullptr;
	}
}
static inline size_t init_args(OpType type) {
	switch (type) {
#define X(name) case OpType::name: return name::init_args();
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("init_args"); else return 0;
	}
}
static inline Op init(OpType type, int* args) {
	switch (type) {
#define X(name) case OpType::name: return name::init(args);
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("init"); else return {};
	}
}
static inline size_t eval_args(const Op* op) {
	switch (op->type) {
#define X(name) case OpType::name: return name::eval_args(op);
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("eval_args"); else return 0;
	}
}
static inline int compute(const Op** op) {
	switch ((*op)->type) {
#define X(name) case OpType::name: return name::compute(op);
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("compute"); else return 0;
	}
}
static inline void compute(const Op* op, const int* args, char* res) {
	switch (op->type) {
#define X(name) case OpType::name: name::compute(op, args, res); break;
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("compute");
	}
}
static inline std::string string_infix(const Op** op) {
	switch ((*op)->type) {
#define X(name) case OpType::name: return name::string_infix(op);
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("string_infix"); else return "";
	}
}
static inline void string_infix(const Op* op, const std::string* args, Uninited<std::string>* res) {
	switch (op->type) {
#define X(name) case OpType::name: name::string_infix(op, args, res); break;
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("string_infix");
	}
}
static inline std::string string_save(const Op** op) {
	switch ((*op)->type) {
#define X(name) case OpType::name: return name::string_save(op);
		OP_TYPES
#undef X
		default: if (fastpath_exceptions) throw std::invalid_argument("string_save"); else return "";
	}
}
static inline std::optional<OpType> type_by_string(const std::string& str) {
#define X(name) if (name::prefix() == str) return OpType::name;
		OP_TYPES
#undef X
	return std::nullopt;
}
}

static inline Expr concat_expr_args(Op op, const std::vector<Expr>& args) {
	if (fastpath_exceptions && ops::eval_args(&op) != args.size()) [[unlikely]] {
		throw std::invalid_argument("invalid arg count for operator");
	}

	Expr result = {op};
	for (auto arg : args) {
		result.insert(result.end(), arg.begin(), arg.end());
	}
	return result;
}

Expr CaExpressionsDriver::ConstantExpression(int value) {
	return {ops::literal::init(&value)};
}

Expr CaExpressionsDriver::BinaryOperatorExpression(char op, Expr l, Expr r) {
	OpType op_type;
	switch (op) {
		case '+': op_type = OpType::plus; break;
		case '-': op_type = OpType::minus; break;
		default: throw std::domain_error("BinaryOperator with unknown operator: " + op); break;
	}
	return concat_expr_args(ops::init(op_type, nullptr), {l, r});
}

Expr CaExpressionsDriver::TernaryOperatorExpression(Expr c, Expr t, Expr f) {
	return concat_expr_args(ops::ternary::init(nullptr), {c, t, f});
}

Expr CaExpressionsDriver::FunctionCallExpression(std::string f, const std::vector<Expr>& args) {
	OpType op_type;
	if (f == "min") op_type = OpType::min;
	else if (f == "max") op_type = OpType::max;
	else {
		if (fastpath_exceptions) throw std::domain_error("function with unknown name: " + f);
		else op_type = OpType::SIZE;
	}
	int argc;
	if (args.size() > std::numeric_limits<decltype(argc)>::max()) {
		throw std::invalid_argument("function has too many arguments: " + f + ", " + std::to_string(args.size()));
	} else {
		argc = static_cast<decltype(argc)>(args.size());
	}
	return concat_expr_args(ops::init(op_type, &argc), args);
}

Expr CaExpressionsDriver::Load(std::istream& s) {
	Expr result;
	std::string multitoken;
	std::string token;
	while (s >> token) {
		if (!multitoken.empty()) multitoken += " ";
		multitoken += token;

		if (debug_print) std::cout << "parse token: " << multitoken << std::endl;

		std::optional<OpType> type = ops::type_by_string(multitoken);
		if (!type) continue;

		if (fastpath_exceptions && ops::init_args(*type) > 1) {
			throw std::logic_error("don't support operators with more than one argument");
		}
		int arg = 0;
		if (ops::init_args(*type) == 1) {
			s >> arg;
		}
		result.push_back(ops::init(*type, &arg));
		multitoken.clear();
	}
	return result;
}

template <typename T, void (*F)(const Op*, const T*, Uninited<T>*)>
void EvaluateOnStack(const Op** op, Uninited<T>* res) {
	const Op* curr_op = *op;
	const size_t args_needed = ops::eval_args(curr_op);

	static const size_t max_local_args = 3;
	alignas(T) char local_args_space[max_local_args*sizeof(T)];
	char* args_space;
	if (args_needed <= max_local_args) [[likely]] {
		args_space = local_args_space;
	} else {
		args_space = new char[args_needed*sizeof(T)];
	}

	for (size_t args_found = 0; args_found < args_needed; ++args_found) {
		++(*op);
		const size_t arg_args_needed = ops::eval_args(*op);
		if (arg_args_needed == 0) {
			F(*op, nullptr, args_space+args_found*sizeof(T));
		} else {
			EvaluateOnStack<T, F>(op, args_space+args_found*sizeof(T));
		}
	}

	T* args = std::launder(reinterpret_cast<T*>(args_space));
	F(curr_op, args, res);

	if (!std::is_trivially_destructible<T>::value) {
		std::destroy_n(args, args_needed);
	}
	if (args_needed > max_local_args) [[unlikely]] {
		delete[] args_space;
	}
}

struct OprPos {
	Op op;
	size_t idx;
};

// In a real example, we'd just want to pass a scratch buffer around
// instead of having a global memory location.
char heap_lit_raw[16*1024*1024];
OprPos heap_opr[16*1024*1024];

template <typename T, void (*F)(const Op*, const T*, Uninited<T>*)>
T EvaluateOnHeap(const Expr* input) {
	T* heap_lit = std::launder(reinterpret_cast<T*>(heap_lit_raw));
	const size_t heap_lit_size = sizeof(heap_lit_raw)/sizeof(*heap_lit);
	const size_t heap_opr_size = sizeof(heap_opr)/sizeof(*heap_opr);
	if (input->size() > heap_lit_size) throw std::domain_error("increase default heap_lit size");
	if (input->size() > heap_opr_size) throw std::domain_error("increase default heap_opr size");

	const Op* op = input->data();
	size_t opr_idx = 0;
	size_t lit_idx = 0;
	do {
		if (ops::eval_args(op) == 0) {
			F(op, nullptr, cast(reinterpret_cast<char*>(heap_lit+(lit_idx++))));
			//std::cout << "added lit " << *op << std::endl;
		} else {
			heap_opr[opr_idx++] = {*op, lit_idx++};
			//std::cout << "added opr " << *op << std::endl;
		}

		while (opr_idx > 0) {
			const OprPos opr = heap_opr[opr_idx-1];
			const size_t argc = ops::eval_args(&opr.op);
			if (lit_idx <= opr.idx + argc) break;

			F(&opr.op, heap_lit+opr.idx+1, cast(reinterpret_cast<char*>(heap_lit+opr.idx)));
			//std::cout << "exec " << heap_lit[opr.idx] << std::endl;
			lit_idx -= argc;
			if (!std::is_trivially_destructible<T>::value) std::destroy_n(heap_lit+lit_idx, argc);
			--opr_idx;
		}

		++op;
	} while (opr_idx != 0);
	if (lit_idx != 1) {
		throw std::invalid_argument("reached end of operators with operands left");
	}
	if (op != input->data() + input->size()) {
		throw std::invalid_argument("used less or more operators than given");
	}
	//std::cout << "done" << std::endl;
	T result = heap_lit[0];
	std::destroy_n(heap_lit, lit_idx);
	return result;
}

int CaExpressionsDriver::Compute(Expr &input) {
	const Op* op = input.data();
	if (fastpath_exceptions && input.empty()) return 0;

	switch (implementation) {
		case Implementation::EvaluateOnStack: {
			int result;
			EvaluateOnStack<int, ops::compute>(&op, cast(&result));
			return result;
		}
		case Implementation::EvaluateOnHeap: {
			return EvaluateOnHeap<int, ops::compute>(&input);
		}
		case Implementation::Recursive: {
			return ops::compute(&op);
		}
		default: throw 0;
	}
}

std::string CaExpressionsDriver::AsString(Expr &input) {
	const Op* op = input.data();
	if (fastpath_exceptions && input.empty()) return 0;

	switch (implementation) {
		case Implementation::EvaluateOnHeap:
		case Implementation::EvaluateOnStack: {
			std::string result;
			EvaluateOnStack<std::string, ops::string_infix>(&op, cast(&result));
			return result;
		}
		case Implementation::Recursive: {
			return ops::string_infix(&op);
		}
		default: throw 0;
	}
}

void CaExpressionsDriver::Save(Expr& input, std::ostream &s) {
	const Op* op = input.data();
	if (fastpath_exceptions && input.empty()) return;

	s << ops::string_save(&op);
}

