
// Helper macros and methods to return and propagate errors with `absl::Status`.

#ifndef COMMON_STATUS_MACROS_H_
#define COMMON_STATUS_MACROS_H_

#include <utility>

#include "absl/base/optimization.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"

// Evaluates an expression that produces a `absl::Status`. If the status is not
// ok, returns it from the current function.
//
// For example:
//   absl::Status MultiStepFunction() {
//     RETURN_IF_ERROR(Function(args...));
//     RETURN_IF_ERROR(foo.Method(args...));
//     return absl::OkStatus();
//   }
#define RETURN_IF_ERROR(expr)                                                  \
  do {                                                                         \
    if (::absl::Status status = (expr); status.ok()) {                         \
    } else {                                                                   \
      return status;                                                           \
    }                                                                          \
  } while (false)

// Executes an expression `rexpr` that returns an `absl::StatusOr<T>`. On OK,
// moves its value into the variable defined by `lhs`, otherwise returns
// from the current function.
//
// Interface:
//
//   ASSIGN_OR_RETURN(lhs, rexpr)
//
// WARNING: if lhs is parenthesized, the parentheses are removed. See examples
// for more details.
//
// WARNING: expands into multiple statements; it cannot be used in a single
// statement (e.g. as the body of an if statement without {})!
//
// Example: Declaring and initializing a new variable (ValueType can be anything
//          that can be initialized with assignment, including references):
//   ASSIGN_OR_RETURN(ValueType value, MaybeGetValue(arg));
//
// Example: Assigning to an existing variable:
//   ValueType value;
//   ASSIGN_OR_RETURN(value, MaybeGetValue(arg));
//
// Example: Assigning to an expression with side effects:
//   MyProto data;
//   ASSIGN_OR_RETURN(*data.mutable_str(), MaybeGetValue(arg));
//   // No field "str" is added on error.
//
// Example: Initializing a `std::unique_ptr`.
//   ASSIGN_OR_RETURN(std::unique_ptr<T> ptr, MaybeGetPtr(arg));
//
// Example: Initializing a map. Because of C++ preprocessor limitations,
// the type used in ASSIGN_OR_RETURN cannot contain commas, so wrap the
// lhs in parentheses:
//   ASSIGN_OR_RETURN((absl::flat_hash_map<Foo, Bar> my_map), GetMap());
// Or use `auto` if the type is obvious enough:
//   ASSIGN_OR_RETURN(auto my_map, GetMap());
//
// Example: Assigning to structured bindings (<internal link>/169). The same
// situation with comma as in map, so wrap the statement in parentheses.
//   ASSIGN_OR_RETURN((auto [first, second]), GetPair());
#define ASSIGN_OR_RETURN(lhs, rexpr)                                           \
  STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_(                                        \
      STATUS_MACROS_IMPL_CONCAT_(_status_or_value, __LINE__), lhs, rexpr,      \
      return ::absl::Status(                                                   \
          ::std::move(STATUS_MACROS_IMPL_CONCAT_(_status_or_value, __LINE__))  \
              .status()))

// =================================================================
// == Implementation details, do not rely on anything below here. ==
// =================================================================
namespace crubit {
// Some builds do not support C++14 fully yet, using C++11 constexpr technique.
constexpr bool HasPotentialConditionalOperator(const char *lhs, int index) {
  return (index == -1
              ? false
              : (lhs[index] == '?'
                     ? true
                     : HasPotentialConditionalOperator(lhs, index - 1)));
}
} // namespace crubit

#define STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_(statusor, lhs, rexpr,             \
                                             error_expression)                 \
  auto statusor = (rexpr);                                                     \
  if (ABSL_PREDICT_FALSE(!statusor.ok())) {                                    \
    error_expression;                                                          \
  }                                                                            \
  {                                                                            \
    static_assert(#lhs[0] != '(' || #lhs[sizeof(#lhs) - 2] != ')' ||           \
                      !::crubit::HasPotentialConditionalOperator(              \
                          #lhs, sizeof(#lhs) - 2),                             \
                  "Identified potential conditional operator, consider not "   \
                  "using ASSIGN_OR_RETURN");                                   \
  }                                                                            \
  STATUS_MACROS_IMPL_UNPARENTHESIZE_IF_PARENTHESIZED(lhs) =                    \
      ::std::move(statusor).value()

// Internal helpers for macro expansion.
#define STATUS_MACROS_IMPL_EAT(...)
#define STATUS_MACROS_IMPL_REM(...) __VA_ARGS__
#define STATUS_MACROS_IMPL_EMPTY()

// Internal helpers for emptyness arguments check.
#define STATUS_MACROS_IMPL_IS_EMPTY_INNER(...)                                 \
  STATUS_MACROS_IMPL_IS_EMPTY_INNER_HELPER((__VA_ARGS__, 0, 1))
// MSVC expands variadic macros incorrectly, so we need this extra indirection
// to work around that (b/110959038).
#define STATUS_MACROS_IMPL_IS_EMPTY_INNER_HELPER(args)                         \
  STATUS_MACROS_IMPL_IS_EMPTY_INNER_I args
#define STATUS_MACROS_IMPL_IS_EMPTY_INNER_I(e0, e1, is_empty, ...) is_empty

#define STATUS_MACROS_IMPL_IS_EMPTY(...)                                       \
  STATUS_MACROS_IMPL_IS_EMPTY_I(__VA_ARGS__)
#define STATUS_MACROS_IMPL_IS_EMPTY_I(...)                                     \
  STATUS_MACROS_IMPL_IS_EMPTY_INNER(_, ##__VA_ARGS__)

// Internal helpers for if statement.
#define STATUS_MACROS_IMPL_IF_1(_Then, _Else) _Then
#define STATUS_MACROS_IMPL_IF_0(_Then, _Else) _Else
#define STATUS_MACROS_IMPL_IF(_Cond, _Then, _Else)                             \
  STATUS_MACROS_IMPL_CONCAT_(STATUS_MACROS_IMPL_IF_, _Cond)                    \
  (_Then, _Else)

// Expands to 1 if the input is parenthesized. Otherwise expands to 0.
#define STATUS_MACROS_IMPL_IS_PARENTHESIZED(...)                               \
  STATUS_MACROS_IMPL_IS_EMPTY(STATUS_MACROS_IMPL_EAT __VA_ARGS__)

// If the input is parenthesized, removes the parentheses. Otherwise expands to
// the input unchanged.
#define STATUS_MACROS_IMPL_UNPARENTHESIZE_IF_PARENTHESIZED(...)                \
  STATUS_MACROS_IMPL_IF(STATUS_MACROS_IMPL_IS_PARENTHESIZED(__VA_ARGS__),      \
                        STATUS_MACROS_IMPL_REM, STATUS_MACROS_IMPL_EMPTY())    \
  __VA_ARGS__

// Internal helper for concatenating macro values.
#define STATUS_MACROS_IMPL_CONCAT_INNER_(x, y) x##y
#define STATUS_MACROS_IMPL_CONCAT_(x, y) STATUS_MACROS_IMPL_CONCAT_INNER_(x, y)

#endif // COMMON_STATUS_MACROS_H_