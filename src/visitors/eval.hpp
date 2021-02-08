#pragma once

#ifndef WOTPP_EVAL
#define WOTPP_EVAL

#include <string>
#include <unordered_map>
#include <stack>
#include <type_traits>
#include <memory>

#include <utils/util.hpp>
#include <structures/ast.hpp>
#include <parser.hpp>

// AST visitor that evaluates the program.

namespace wpp {
	using Environment = std::unordered_map<std::string, wpp::node_t>;
	using Arguments = std::unordered_map<std::string, wpp::node_t>;


	template <typename... Ts>
	inline std::string mangle(Ts&&... args) {
		const auto tostr = [] (const auto& x) {
			if constexpr(std::is_same_v<std::decay_t<decltype(x)>, std::string>)
				return x;

			else
				return std::to_string(x);
		};

		return (tostr(args) + ...);
	}


	inline std::string eval_ast(
		const wpp::node_t node_id,
		const wpp::AST& tree,
		Environment& functions,
		std::unique_ptr<Arguments>&& args = nullptr
	) {
		// Fetch variant.
		const auto& variant = tree[node_id];
		std::string str; // Return value.


		// Visit the variant.
		wpp::visit(variant,
			[&] (const FnInvoke& call) {
				const auto& [caller_name, caller_args, caller_pos] = call;
				std::string caller_mangled_name = mangle(caller_name, caller_args.size());

				tinge::noticeln(tinge::before{"\n"}, " function call: ", caller_mangled_name);


				// Check if caller is an argument.
				if (args) {
					tinge::noticeln("*args is not nullptr");

					// Lookup caller_name in `args` map.
					if (auto it = args->find(caller_name); it != args->end()) {
						tinge::noticeln(caller_name, " is an argument.");
						// If we find something, evaluate it.
						str = eval_ast(it->second, tree, functions, std::move(args));
						return; // Early out.
					}
				}


				// If the caller is not an argument, it might be a function.
				// Check if it's an intrinsic.
				if (caller_name == "run") {
					// Set up arguments in environment.
					std::string command;

					for (int i = 0; i < (int)caller_args.size(); i++) {
						auto retstr = eval_ast(caller_args[i], tree, functions, std::move(args));
						command += retstr + " ";
					}

					// Run the command
					str = wpp::exec(command);
				}


				else {
					// Lookup caller_name to see if it's a function.
					auto it = functions.find(caller_mangled_name);
					if (it == functions.end())
						wpp::error(caller_pos, "func not found: ", caller_mangled_name);

					tinge::warnln(caller_mangled_name, " is a function");


					// Fetch the AST node of the function.
					const auto& [callee_name, params, body, callee_pos] = tree.get<wpp::Fn>(it->second);


					// If args is a nullptr, setup a new Arguments structure to pass down.
					if (not args) {
						tinge::warnln("creating new argument structure.");
						args = std::make_unique<Arguments>();
					}


					// Map all arguments to parameter name -> node id of argument
					for (int i = 0; i < (int)caller_args.size(); i++)
						args->emplace(params[i], caller_args[i]);


					// Call function.
					str = eval_ast(body, tree, functions, std::move(args));
				}
			},

			[&] (const Fn& func) {
				const auto& [name, params, body, pos] = func;
				functions.insert_or_assign(mangle(name, params.size()), node_id);
			},

			[&] (const String& x) {
				const auto& [literal, pos] = x;
				str = literal;
			},

			[&] (const Concat& cat) {
				const auto& [lhs, rhs, pos] = cat;
				str = eval_ast(lhs, tree, functions, std::move(args)) + eval_ast(rhs, tree, functions, std::move(args));
			},

			[&] (const Block& block) {
				const auto& [stmts, expr, pos] = block;

				for (const wpp::node_t node: stmts) {
					str += eval_ast(node, tree, functions, std::move(args));
				}

				str = eval_ast(expr, tree, functions, std::move(args));
			},

			[&] (const Ns&) {
				// const auto& [name, stmts] = ns;
			},

			[&] (const Document& doc) {
				for (const wpp::node_t node: doc.exprs_or_stmts) {
					str += eval_ast(node, tree, functions, std::move(args));
				}
			}
		);

		return str;
	}
}

#endif

