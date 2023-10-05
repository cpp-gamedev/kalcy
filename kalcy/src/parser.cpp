#include <kalcy/parser.hpp>
#include <kalcy/scanner.hpp>
#include <cassert>

namespace kalcy {
Parser::Parser(std::string_view text) : m_scanner(text), m_current(m_scanner.next()), m_next(m_scanner.next()) {}

auto Parser::parse() noexcept(false) -> UExpr {
	if (!m_current) { return {}; }
	auto ret = expression();
	if (m_current || m_next) {
		auto const extranous = m_current ? m_current : m_next;
		throw Error{.token = extranous, .expected = Token::Type::eNone};
	}
	return ret;
}

auto Parser::expression() -> UExpr { return sum(); }

auto Parser::sum() -> UExpr {
	auto ret = product();

	while (match(Token::Type::ePlus, Token::Type::eMinus)) {
		auto const token = m_previous;
		auto rhs = expression();
		ret = std::make_unique<Expr>(expr::Binary{.left = std::move(ret), .op = token, .right = std::move(rhs)});
	}

	return ret;
}

auto Parser::product() -> UExpr {
	auto ret = exponent();

	while (match(Token::Type::eStar, Token::Type::eSlash)) {
		auto const token = m_previous;
		auto rhs = exponent();
		ret = std::make_unique<Expr>(expr::Binary{.left = std::move(ret), .op = token, .right = std::move(rhs)});
	}

	return ret;
}

auto Parser::exponent() -> UExpr {
	auto ret = unary();

	while (match(Token::Type::eCaret)) {
		auto const token = m_previous;
		auto rhs = unary();
		ret = std::make_unique<Expr>(expr::Binary{.left = std::move(ret), .op = token, .right = std::move(rhs)});
	}

	return ret;
}

auto Parser::unary() -> UExpr {
	if (match(Token::Type::eMinus)) {
		auto const token = m_previous;
		auto rhs = unary();
		return std::make_unique<Expr>(expr::Unary{.op = token, .right = std::move(rhs)});
	}

	return call();
}

auto Parser::call() -> UExpr {
	auto ret = primary();
	if (match(Token::Type::eParenLeft)) {
		auto fn = expr::Call{.callee = std::move(ret), .paren_l = m_previous};
		if (m_current.type != Token::Type::eParenRight) {
			do { fn.arguments.push_back(expression()); } while (match(Token::Type::eComma)); // NOLINT
		}
		consume(Token::Type::eParenRight);
		fn.paren_r = m_previous;
		ret = std::make_unique<Expr>(std::move(fn));
	}
	return ret;
}

auto Parser::primary() -> UExpr {
	if (match(Token::Type::eNumber, Token::Type::eIdentifier)) { return std::make_unique<Expr>(expr::Literal{.token = m_previous}); }

	if (match(Token::Type::eParenLeft)) {
		auto ret = expression();
		consume(Token::Type::eParenRight);
		return std::make_unique<Expr>(expr::Group{.expr = std::move(ret)});
	}

	throw Error{.token = m_current};
}

template <typename... T>
auto Parser::match(T... types) -> bool {
	auto const token = m_current;
	if ((... || (token.type == types))) {
		advance();
		return true;
	}
	return false;
}

auto Parser::consume(Token::Type type) noexcept(false) -> void {
	if (!match(type)) { throw Error{.token = m_current, .expected = type}; }
}

void Parser::advance() {
	m_previous = m_current;
	m_current = m_next;
	m_next = m_scanner.next();
}
} // namespace kalcy
