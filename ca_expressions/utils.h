#include "expressions.h"

using namespace ca;

static size_t traced_constr_counter = 0;
static size_t traced_copy_constr_counter = 0;
static size_t traced_copy_ass_counter = 0;
static size_t traced_move_constr_counter = 0;
static size_t traced_move_ass_counter = 0;
static size_t traced_destr_counter = 0;

template <typename T>
struct Traced: T
{
public:
	Traced(const T& t): T(t)  { ++traced_constr_counter; }
	Traced(const Traced&) { ++traced_copy_constr_counter; }
	Traced& operator=(const Traced&) { ++traced_copy_ass_counter; return *this; }
	Traced(Traced&&) noexcept { ++traced_move_constr_counter; }
	Traced& operator=(Traced&&) noexcept { ++traced_move_ass_counter; return *this; }
	~Traced() { ++traced_destr_counter; }
};

std::ostream& operator<<(std::ostream& o, const Op& op) {
	o << "{" << static_cast<short>(op.type) << "," << op.data.literal.number << "}";
	return o;
}

template <typename T, typename A>
std::ostream& operator<<(std::ostream& o, const std::vector<T, A>& v) {
	o << "[\n";
	for (const auto& x : v) {
		o << x << "\n";
	}
	o << "]\n";
	return o;
}

#ifdef TRACE
void __attribute__((destructor)) dtor() {
    std::cout << "cstr " << traced_constr_counter << std::endl;
    std::cout << "copy_cstr " << traced_copy_constr_counter << std::endl;
    std::cout << "copy_ass " << traced_copy_ass_counter << std::endl;
    std::cout << "move_cstr " << traced_move_constr_counter << std::endl;
    std::cout << "move_ass " << traced_move_ass_counter << std::endl;
    std::cout << "dstr " << traced_destr_counter << std::endl;
}
#endif
